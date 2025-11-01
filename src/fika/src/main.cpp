/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/process.hpp>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "cli.h"
#include "cli_parser.h"
#include "commands.h"
#include "config.h"
#include "msg.h"
#include "pid_file.h"

// TODO(samuil): the hardcodes string "/tmp/fika_supervisor.pid" should be taken
// from a config file
bool is_supervisor_running() {
  std::string pid_path("/tmp/fika_supervisor.pid");
  bool locked = fika::util::pidfile_lock::is_locked(pid_path);

  if (!locked) {
    fika::log::log_info("No supervisor lock, starting a new one.");
    return false;
  }

  try {
    // Must match the queue name supervisor creates
    boost::interprocess::message_queue mq_(boost::interprocess::open_only,
                                           "fika_supervisor_mq");
    boost::interprocess::message_queue mq_recv(boost::interprocess::open_only,
                                               "supervisor_fika_mq");

    fika::CommandMessage msg{fika::CommandType::Ping};
    mq_.send(&msg, sizeof(msg), 0);

    std::size_t recv_size{};
    unsigned int priority{};
    fika::CommandResponse response;
    // TODO(samuil): a ping with id should be send and check if the returned id
    // is the same this will prevent a stale queue from having garbage or
    // ingeneral try and drain the message queue
    auto abs_time =
        boost::chrono::system_clock::now() + boost::chrono::seconds(20);

    if (mq_recv.timed_receive(&response, sizeof(response), recv_size, priority,
                              abs_time)) {
      fika::log::log_info("Supervisor is alive... Continue no init needed.");
      return true;
    }

    fika::log::log_info(
        "Supervisor is dead and queues are in a good state... Starting "
        "supervisor service!");

    return false;
  } catch (const boost::interprocess::interprocess_exception &ex) {
    fika::log::log_warn(
        "Supervisor is dead or queues are in a bad state. Starting supervisor "
        "service!");
    return false;
  }
}

void start_supervisor() {
  boost::interprocess::message_queue::remove("fika_supervisor_mq");
  boost::interprocess::message_queue::remove("supervisor_fika_mq");

  auto sup = boost::process::child(std::string(BINARIES_LOC) + "/supervisor",
                                   "--pidfile=/tmp/fika_supervisor.pid",
                                   boost::process::std_out > stdout,
                                   boost::process::std_err > stderr);

  sup.detach();  // supervisor runs independently
}

void send_command(const fika::CommandMessage &msg) {
  boost::interprocess::message_queue mq_(boost::interprocess::open_or_create,
                                         "fika_supervisor_mq", 100,
                                         sizeof(fika::CommandMessage));
  mq_.send(&msg, sizeof(msg), 0);
}

int main(int argc, char *argv[]) {
  fika::cli_parser cli(argc, argv);
  cli.initialize(fika::default_option::help | fika::default_option::version);
  cli.set_options_description(fika::cli::command_description());
  auto vm = cli.parse();
  if (!vm.has_value()) {
    return -1;
  }

  // TODO(samuil): the parse_command_line should become a callback and let the
  // cli_parser call the result will be a command and service that is then
  // passed to send_command
  fika::cli::ParsedCommand parsed;
  try {
    parsed = fika::cli::parse_command_line(vm.value());
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    cli.print_help();
    return EXIT_FAILURE;
  }

  if (!is_supervisor_running()) {
    start_supervisor();
  }

  send_command(fika::CommandMessage(parsed.type, parsed.service));
  return EXIT_SUCCESS;
}
