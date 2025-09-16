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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <cstring>
#include <string>

#include "../include/commands.h"

using namespace fika;

TEST_CASE("CommandMessage default constructor") {
  CommandMessage msg;
  CHECK(msg.cmd == CommandType::Start);
  CHECK(msg.service() == "");
  CHECK(msg.service_name[0] == '\0');  // null-terminated
}

TEST_CASE("CommandMessage custom constructor") {
  CommandMessage msg(CommandType::Stop, "nginx");
  CHECK(msg.cmd == CommandType::Stop);
  CHECK(msg.service() == "nginx");
  CHECK(std::strcmp(msg.service_name, "nginx") == 0);
}

TEST_CASE("CommandMessage truncates long service name") {
  std::string long_name(100, 'A');  // longer than 64 chars
  CommandMessage msg(CommandType::Restart, long_name);

  // Should be truncated to 63 characters + null terminator
  CHECK(std::strlen(msg.service_name) == sizeof(msg.service_name) - 1);
  CHECK(msg.service_name[sizeof(msg.service_name) - 1] == '\0');

  std::string truncated = msg.service();
  CHECK(truncated.size() == sizeof(msg.service_name) - 1);
  CHECK(truncated.find('A') != std::string::npos);
}

TEST_CASE("CommandResponse default constructor") {
  CommandResponse resp;
  CHECK(resp.success == false);
  CHECK(resp.message[0] == '\0');  // null-terminated
}

TEST_CASE("CommandResponse message assignment") {
  CommandResponse resp;
  resp.success = true;
  std::strncpy(resp.message, "Service started", sizeof(resp.message) - 1);
  resp.message[sizeof(resp.message) - 1] = '\0';

  CHECK(resp.success == true);
  CHECK(std::string(resp.message) == "Service started");
  CHECK(resp.message[sizeof(resp.message) - 1] ==
        '\0');  // still null-terminated
}
