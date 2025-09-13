/*
 * This file is part of Fika.
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

#include "commands.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;
namespace bip = boost::interprocess;

void send_command(const fika::CommandMessage &msg) {
  bip::message_queue mq(bip::open_or_create, "fika_supervisor_mq", 100,
                        sizeof(fika::CommandMessage));
  mq.send(&msg, sizeof(msg), 0);
}

int main(int argc, char *argv[]) {
  po::options_description desc("Commands");
  desc.add_options()("help,h", "show help")(
      "command,c", po::value<std::string>(),
      "command to execute (start|stop|restart|reload|status|logs)")(
      "service,s", po::value<std::string>(), "service name (optional)");

  po::positional_options_description p;
  p.add("command", 1);

  po::variables_map vm;
  try {
    po::store(
        po::command_line_parser(argc, argv).options(desc).positional(p).run(),
        vm);
    po::notify(vm);
  } catch (const boost::program_options::error &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    std::cerr << desc << "\n";
    return EXIT_FAILURE;
  }

  if (vm.count("help") || !vm.count("command")) {
    std::cout << desc << "\n";
    return 0;
  }

  std::string cmd_str = vm["command"].as<std::string>();
  std::string service =
      vm.count("service") ? vm["service"].as<std::string>() : "";

  fika::CommandType cmd;
  if (cmd_str == "start")
    cmd = fika::CommandType::Start;
  else if (cmd_str == "stop")
    cmd = fika::CommandType::Stop;
  else if (cmd_str == "restart")
    cmd = fika::CommandType::Restart;
  else if (cmd_str == "reload")
    cmd = fika::CommandType::Reload;
  else if (cmd_str == "status")
    cmd = fika::CommandType::Status;
  else if (cmd_str == "logs")
    cmd = fika::CommandType::Logs;
  else {
    std::cerr << "Unknown command\n";
    return 1;
  }

  send_command(fika::CommandMessage(cmd, service));
  return 0;
}
