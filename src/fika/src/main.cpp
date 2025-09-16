/*
 * This file is part of Fika.
 *
 * Copyright [2025] Samuil Ivanov
 *
 * Fika is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * Fika is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fika.  If not, see <http://www.gnu.org/licenses/>.
 */

// clang-format off
#include <iostream>

#include "cli.h"
#include "commands.h"
#include "config.h"
#include "msg.h"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/process.hpp>
// clang-format on

bool is_supervisor_running() {
  try {
    // Must match the queue name supervisor creates
    boost::interprocess::message_queue mq(boost::interprocess::open_only,
                                          "fika_supervisor_mq");
    boost::interprocess::message_queue mq_recv(boost::interprocess::open_only,
                                               "supervisor_fika_mq");

    fika::CommandMessage msg{fika::CommandType::Ping};
    mq.send(&msg, sizeof(msg), 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::size_t recv_size;
    unsigned int priority;
    fika::CommandResponse r;

    if (mq_recv.try_receive(&r, sizeof(r), recv_size, priority)) {
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

boost::process::child start_supervisor() {
  return boost::process::child(
      std::string(BINARIES_LOC) + "/supervisor",
      boost::process::std_out > stdout,
      boost::process::std_err >
          stderr);  // assumes "supervisor" binary is on PATH
}

void send_command(const fika::CommandMessage &msg) {
  boost::interprocess::message_queue mq(boost::interprocess::open_or_create,
                                        "fika_supervisor_mq", 100,
                                        sizeof(fika::CommandMessage));
  mq.send(&msg, sizeof(msg), 0);
}

int main(int argc, char *argv[]) {
  boost::process::child sup;
  if (!is_supervisor_running()) {
    sup = start_supervisor();
    sup.detach();  // supervisor keeps running after fika exits

    // give it some time to initialize IPC
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  try {
    auto parsed = fika::cli::parse_command_line(argc, argv);
    send_command(fika::CommandMessage(parsed.type, parsed.service));
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
