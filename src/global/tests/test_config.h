/*
 * This file is part of file_ingest_key_analyser (FIKA).
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

#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
class test_config {
 public:
  std::string host;
  int port;

  static constexpr std::string_view key = "server";

  // Static factory method
  static test_config fromJson(const nlohmann::json &root) {
    test_config cfg;
    const auto &j = root.at(key);
    cfg.host = j.value("host", "127.0.0.1");  // default host
    cfg.port = j.value("port", 8080);         // default port
    return cfg;
  }
};

#endif