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
#include <boost/program_options/variables_map.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>

namespace fika {

enum class default_option : unsigned {
  none = 0,
  pidfile = 1 << 0,
  verbose = 1 << 1,
  version = 1 << 2,
  help = 1 << 3,
  all = pidfile | verbose | version | help
};

// Enable bitwise operators
inline default_option operator|(default_option a, default_option b) {
  return static_cast<default_option>(static_cast<unsigned>(a) |
                                     static_cast<unsigned>(b));
}
inline bool has_flag(default_option flags, default_option flag) {
  return (static_cast<unsigned>(flags) & static_cast<unsigned>(flag)) != 0;
}

// Generic CLI parser, reusable for any app
class cli_parser {
 public:
  void initialize(default_option flags = default_option::all);
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

  template <typename T>
  [[nodiscard]] T get_option(const std::string& name) {
    if (!vm_.count(name)) {
      throw std::runtime_error("Option not found: " + name);
    }
    return vm_[name].as<T>();
  }

  [[nodiscard]] std::optional<boost::program_options::variables_map> parse();

  void print_help() const;
  void set_options_description(
      const boost::program_options::options_description& desc);

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
  boost::program_options::variables_map vm_;
};
}  // namespace fika
