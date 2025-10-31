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
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>

namespace fika {

// Generic CLI parser, reusable for any app
class cli_parser {
 public:
  using callback_t =
      std::function<void(const boost::program_options::variables_map&)>;

  cli_parser(int argc, char** argv) : argc_(argc), argv_(argv) {}

  // Add argument with optional callback
  template <typename T = std::string>
  void add_option(const std::string& name, const std::string& description,
                  std::function<void(const T&)> callback = nullptr,
                  T default_value = T()) {
    if constexpr (std::is_same_v<T, bool>) {
      desc_.add_options()(
          name.c_str(),
          boost::program_options::bool_switch()->default_value(false),
          description.c_str());
      if (callback) {
        bool_callbacks_[name] =
            [callback, name](const boost::program_options::variables_map& vm) {
              if (vm.count(name)) {
                callback(vm[name].as<bool>());
              }
            };
      }
    } else {
      desc_.add_options()(
          name.c_str(),
          boost::program_options::value<T>()->default_value(default_value),
          description.c_str());
      if (callback) {
        value_callbacks_[name] =
            [callback, name](const boost::program_options::variables_map& vm) {
              if (vm.count(name)) {
                callback(vm[name].as<T>());
              }
            };
      }
    }
  }

  boost::program_options::variables_map parse();

  void print_help() const;

 private:
  int argc_;
  char** argv_;
  boost::program_options::options_description desc_{"Allowed options"};

  std::map<std::string,
           std::function<void(const boost::program_options::variables_map&)>>
      value_callbacks_;
  std::map<std::string,
           std::function<void(const boost::program_options::variables_map&)>>
      bool_callbacks_;
};
}  // namespace fika
