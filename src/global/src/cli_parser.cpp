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
#include <boost/program_options/value_semantic.hpp>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>

namespace fika {

void cli_parser::initialize(default_option flags) {
  std::string binary_name = std::filesystem::path(argv_[0]).filename().string();

  if (has_flag(flags, default_option::pidfile)) {
    std::string pidfile_description =
        "Store " + binary_name + " pid to {file}.";
    desc_.add_options()(
        "pidfile", boost::program_options::value<std::string>()->required(),
        pidfile_description.c_str());
  }

  if (has_flag(flags, default_option::verbose)) {
    desc_.add_options()(
        "verbose", boost::program_options::bool_switch()->default_value(false),
        "Enable verbose output");
  }

  if (has_flag(flags, default_option::version)) {
    desc_.add_options()(
        "version", boost::program_options::bool_switch()->default_value(false),
        "Shows the version and quits.");
  }

  if (has_flag(flags, default_option::help)) {
    desc_.add_options()("help,h", "Show help message");
  }
}

std::optional<boost::program_options::variables_map> cli_parser::parse() {
  try {
    boost::program_options::store(
        boost::program_options::parse_command_line(argc_, argv_, desc_), vm_);
    boost::program_options::notify(vm_);
  } catch (const boost::program_options::error& e) {
    std::cerr << "CLI error: " << e.what() << "\n";
    print_help();
    return std::nullopt;
  }

  if (vm_.count("help") == 1) {
    print_help();
    return std::nullopt;
  }

  // TODO(samuil): version numbering is not implemented
  if (vm_.count("version") == 1 && vm_["version"].as<bool>()) {
    std::cout << "PRINT VERSION\n";
    return std::nullopt;
  }

  // Call value callbacks
  for (auto& [name, cb] : value_callbacks_) {
    cb(vm_);
  }

  // Call bool callbacks
  for (auto& [name, cb] : bool_callbacks_) {
    cb(vm_);
  }

  return vm_;
}

void cli_parser::print_help() const { std::cout << desc_ << "\n"; }
void cli_parser::set_options_description(
    const boost::program_options::options_description& desc) {
  desc_.add(desc);  // merge the new options into the existing desc_
}
}  // namespace fika
