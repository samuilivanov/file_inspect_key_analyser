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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <memory>

#include "cmd_def.h"
#include "mock_supervisor.h"

TEST_CASE("start_command executes start_workers") {
  std::unique_ptr<test_supervisor> sup = std::make_unique<test_supervisor>();
  fika::detail::start_command cmd;

  cmd.execute(sup.get(), "parser");

  CHECK(sup->started.size() == 1);
  CHECK(sup->started[0] == "parser");
  CHECK(sup->stopped.empty());
}

TEST_CASE("stop_command executes stop_workers") {
  std::unique_ptr<test_supervisor> sup = std::make_unique<test_supervisor>();
  fika::detail::stop_command cmd;

  cmd.execute(sup.get(), "parser");

  CHECK(sup->stopped.size() == 1);
  CHECK(sup->stopped[0] == "parser");
  CHECK(sup->started.empty());
}

TEST_CASE("restart_command stops and starts workers") {
  std::unique_ptr<test_supervisor> sup = std::make_unique<test_supervisor>();
  fika::detail::restart_command cmd;

  cmd.execute(sup.get(), "parser");

  CHECK(sup->stopped.size() == 1);
  CHECK(sup->stopped[0] == "parser");

  CHECK(sup->started.size() == 1);
  CHECK(sup->started[0] == "parser");
}

TEST_CASE("ping_command executes send_pong with pong message") {
  std::unique_ptr<test_supervisor> sup = std::make_unique<test_supervisor>();

  fika::detail::ping_command cmd;

  cmd.execute(sup.get(), "any_service");

  REQUIRE(sup->sent_messages.size() == 1);
  CHECK(sup->sent_messages[0].success == true);
  CHECK(std::string(sup->sent_messages[0].message.data()) == "pong");
}