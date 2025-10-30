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

// clang-format off
#include "cli.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include <boost/program_options.hpp>
// clang-format on

namespace fika::cli {

static const std::map<std::string, fika::CommandType> command_map = {
    {"start", fika::CommandType::Start},
    {"stop", fika::CommandType::Stop},
    {"restart", fika::CommandType::Restart},
    {"reload", fika::CommandType::Reload},
    {"status", fika::CommandType::Status},
    {"logs", fika::CommandType::Logs},
};

ParsedCommand parse_command_line(const std::span<char*> args) {
  boost::program_options::options_description desc("Commands");
  desc.add_options()("help,h", "show help")(
      "command,c", boost::program_options::value<std::string>(),
      "command to execute (start|stop|restart|reload|status|logs)")(
      "service,s", boost::program_options::value<std::string>(),
      "service name (optional)");

  boost::program_options::positional_options_description posd;
  posd.add("command", 1);

  boost::program_options::variables_map var_map;
  boost::program_options::store(boost::program_options::command_line_parser(
                                    static_cast<int>(args.size()), args.data())
                                    .options(desc)
                                    .positional(posd)
                                    .run(),
                                var_map);
  boost::program_options::notify(var_map);

  if (var_map.count("help") || !var_map.count("command")) {
    std::ostringstream oss;
    oss << desc;
    throw std::invalid_argument(oss.str());
  }

  auto cmd_str = var_map["command"].as<std::string>();
  auto iter = command_map.find(cmd_str);
  if (iter == command_map.end()) {
    throw std::invalid_argument("Unknown command: " + cmd_str);
  }

  std::string service =
      var_map.count("service") != 0 ? var_map["service"].as<std::string>() : "";
  return {iter->second, service};
}

}  // namespace fika::cli
