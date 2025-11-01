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

#pragma once
#include <boost/program_options.hpp>
#include <span>
#include <string>

#include "commands.h"  // for fika::CommandType

namespace fika::cli {

struct ParsedCommand {
  fika::CommandType type;
  std::string service;
};

ParsedCommand parse_command_line(
    const boost::program_options::variables_map &var_map);

std::string get_cli_help();
boost::program_options::options_description command_description();
}  // namespace fika::cli
