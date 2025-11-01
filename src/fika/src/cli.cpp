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

#include "cli.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include "commands.h"

namespace fika::cli {

static const std::map<std::string, fika::CommandType> command_map = {
    {"start", fika::CommandType::Start},
    {"stop", fika::CommandType::Stop},
    {"restart", fika::CommandType::Restart},
    {"reload", fika::CommandType::Reload},
    {"status", fika::CommandType::Status},
    {"logs", fika::CommandType::Logs}};

ParsedCommand parse_command_line(
    const boost::program_options::variables_map &var_map) {
  auto cmd_str = var_map["command"].as<std::string>();
  auto iter = command_map.find(cmd_str);
  if (iter == command_map.end()) {
    throw std::invalid_argument("Unknown command: " + cmd_str);
  }

  std::string service =
      var_map.count("service") != 0 ? var_map["service"].as<std::string>() : "";
  return {iter->second, service};
}

boost::program_options::options_description command_description() {
  boost::program_options::options_description desc("Commands");
  desc.add_options()(
      "command,c", boost::program_options::value<std::string>()->required(),
      "command to execute (start|stop|restart|reload|status|logs)")(
      "service,s", boost::program_options::value<std::string>(),
      "service name (optional)");
  return desc;
}

}  // namespace fika::cli
