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

#pragma once
#include "commands.h" // for fika::CommandType
#include <string>

namespace fika::cli {

struct ParsedCommand {
  fika::CommandType type;
  std::string service;
};

/// Parse argc/argv and return a ParsedCommand.
/// Throws std::invalid_argument or boost::program_options::error on failure.
ParsedCommand parse_command_line(int argc, char *argv[]);

} // namespace fika::cli
