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

#ifndef SRC_GLOBAL_INCLUDE_COMMANDS_H_
#define SRC_GLOBAL_INCLUDE_COMMANDS_H_
#include <array>
#include <cstdint>
#include <cstring>
#include <string>

namespace fika {

enum class CommandType : std::uint8_t {
  Start,
  Stop,
  Restart,
  Reload,
  Status,
  Logs,
  Ping
};

constexpr size_t SERVICE_NAME_MAX_LEN = 64;
constexpr size_t SERVICE_RESPONSE_LEN = 256;

struct CommandMessage {
  CommandType cmd_;
  std::array<char, SERVICE_NAME_MAX_LEN> service_name{};

  explicit CommandMessage(CommandType cmd = CommandType::Start,
                          const std::string &service = "")
      : cmd_(cmd) {
    auto len = service.copy(service_name.data(), service_name.size() - 1);
    service_name.at(len) = '\0';
  }

  [[nodiscard]] std::string service() const {
    return std::string{service_name.data()};
  }
};

struct CommandResponse {
  bool success{false};
  std::array<char, SERVICE_RESPONSE_LEN>
      message{};  // e.g., "Service started", "Error: ..."
};

}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_COMMANDS_H_
