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

#ifndef SRC_SUPERVISOR_INCLUDE_WORKER_CONFIGS_H_
#define SRC_SUPERVISOR_INCLUDE_WORKER_CONFIGS_H_

// clang-format off
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
// clang-format on

namespace fika {
struct worker_config {
  std::string name;
  std::string path;
  std::vector<std::string> args;

  static worker_config fromJson(const nlohmann::json &j) {
    worker_config wc;
    wc.name = j.at("name").get<std::string>();
    wc.path = j.at("path").get<std::string>();
    wc.args = j.at("args").get<std::vector<std::string>>();
    return wc;
  }
};

struct worker_configs {
  std::vector<worker_config> workers;

  static worker_configs fromJson(const nlohmann::json &j) {
    worker_configs cfgs;
    for (const auto &w : j) {
      cfgs.workers.push_back(worker_config::fromJson(w));
    }
    return cfgs;
  }
};
}  // namespace fika

#endif  // SRC_SUPERVISOR_INCLUDE_WORKER_CONFIGS_H_
