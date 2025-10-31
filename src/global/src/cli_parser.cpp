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

#include "cli_parser.h"

#include <boost/program_options.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>

namespace fika {

// Generic CLI parser, reusable for any app
// Parse args, run callbacks
boost::program_options::variables_map cli_parser::parse() {
  boost::program_options::variables_map vm;
  try {
    boost::program_options::store(
        boost::program_options::parse_command_line(argc_, argv_, desc_), vm);
    boost::program_options::notify(vm);
  } catch (const boost::program_options::error& e) {
    print_help();
  }

  if (vm.count("help")) {
    print_help();
  }

  // Call value callbacks
  for (auto& [name, cb] : value_callbacks_) {
    cb(vm);
  }

  // Call bool callbacks
  for (auto& [name, cb] : bool_callbacks_) {
    cb(vm);
  }

  return vm;
}

void cli_parser::print_help() const { std::cout << desc_ << "\n"; }

}  // namespace fika
