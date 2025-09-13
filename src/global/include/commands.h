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

#ifndef COMMANDS_H
#define COMMANDS_H
#include <cstring>
#include <string>

namespace fika {

enum class CommandType : int { Start, Stop, Restart, Reload, Status, Logs };

struct CommandMessage {
  CommandType cmd;
  char service_name[64];

  CommandMessage(CommandType c = CommandType::Start,
                 const std::string &service = "")
      : cmd(c) {
    std::strncpy(service_name, service.c_str(), sizeof(service_name) - 1);
    service_name[sizeof(service_name) - 1] = '\0';
  }

  std::string service() const { return std::string(service_name); }
};

struct CommandResponse {
  bool success;
  char message[256]; // e.g., "Service started", "Error: ..."

  CommandResponse() : success(false) { message[0] = '\0'; }
};

} // namespace fika

#endif