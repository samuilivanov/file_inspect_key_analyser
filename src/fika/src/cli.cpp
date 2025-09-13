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

#include "cli.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <map>
#include <stdexcept>

namespace po = boost::program_options;

namespace fika::cli {

static const std::map<std::string, fika::CommandType> command_map = {
    {"start", fika::CommandType::Start},
    {"stop", fika::CommandType::Stop},
    {"restart", fika::CommandType::Restart},
    {"reload", fika::CommandType::Reload},
    {"status", fika::CommandType::Status},
    {"logs", fika::CommandType::Logs},
};

ParsedCommand parse_command_line(int argc, char *argv[]) {
  po::options_description desc("Commands");
  desc.add_options()("help,h", "show help")(
      "command,c", po::value<std::string>(),
      "command to execute (start|stop|restart|reload|status|logs)")(
      "service,s", po::value<std::string>(), "service name (optional)");

  po::positional_options_description p;
  p.add("command", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(),
      vm);
  po::notify(vm);

  if (vm.count("help") || !vm.count("command")) {
    std::ostringstream oss;
    oss << desc;
    throw std::invalid_argument(oss.str());
  }

  auto cmd_str = vm["command"].as<std::string>();
  auto it = command_map.find(cmd_str);
  if (it == command_map.end()) {
    throw std::invalid_argument("Unknown command: " + cmd_str);
  }

  std::string service =
      vm.count("service") ? vm["service"].as<std::string>() : "";
  return {it->second, service};
}

} // namespace fika::cli
