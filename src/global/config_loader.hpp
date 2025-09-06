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

#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace fika::config {

// TODO (samuil) Add a config path resolver based on the enviroment
// at the moment it's just hardcoded
template <typename T>
inline T load(const std::string &filename, const std::string &json_key = "") {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Config file not found: " + filename);
  }
  nlohmann::json j;
  file >> j;
  std::cout << "Loaded JSON:\n" << j.dump(4) << std::endl; // debug
  if (json_key.empty()) {
    return T::fromJson(j);
  } else {
    return T::fromJson(j.at(json_key));
  }
}

} // namespace fika::config

#endif