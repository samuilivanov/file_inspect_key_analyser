/*
 * This file is part of Fika.
 *
 * Copyright [2025] Samuil Ivanov
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

#ifndef SRC_SUPERVISOR_INCLUDE_CMD_DEF_H_
#define SRC_SUPERVISOR_INCLUDE_CMD_DEF_H_

#include <string>

#include "cmd.h"
#include "supervisor.h"

namespace fika::detail {
class start_command : public cmd {
 public:
  void execute(supervisor *sup, const std::string &service_name = "") override {
    sup->start_workers(service_name);  // optionally filter by service_name
  }
};

class stop_command : public cmd {
 public:
  void execute(supervisor *sup, const std::string &service_name = "") override {
    sup->stop_workers(service_name);  // you would add stop_workers method
  }
};

class restart_command : public cmd {
 public:
  void execute(supervisor *sup, const std::string &service_name = "") override {
    sup->stop_workers(service_name);
    sup->start_workers(service_name);
  }
};

class ping_command : public cmd {
 public:
  void execute(supervisor *sup, const std::string &service_name = "") override {
    CommandResponse msg;
    msg.success = true;
    std::strcpy(msg.message.data(), "pong");
    sup->send_pong(msg);
  }
};
}  // namespace fika::detail

#endif  // SRC_SUPERVISOR_INCLUDE_CMD_DEF_H_
