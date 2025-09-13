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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cli.h"
#include <doctest/doctest.h>

TEST_CASE("parse start command with service") {
  const char *argv[] = {"fika", "-c", "start", "-s", "parser"};
  int argc = 5;
  auto result = fika::cli::parse_command_line(argc, const_cast<char **>(argv));
  CHECK(result.type == fika::CommandType::Start);
  CHECK(result.service == "parser");
}

TEST_CASE("parse stop command without service") {
  const char *argv[] = {"fika", "-c", "stop"};
  int argc = 3;
  auto result = fika::cli::parse_command_line(argc, const_cast<char **>(argv));
  CHECK(result.type == fika::CommandType::Stop);
  CHECK(result.service.empty());
}

TEST_CASE("unknown command throws") {
  const char *argv[] = {"fika", "foo"};
  int argc = 2;
  CHECK_THROWS_AS(
      fika::cli::parse_command_line(argc, const_cast<char **>(argv)),
      std::invalid_argument);
}

TEST_CASE("help prints usage") {
  const char *argv[] = {"fika", "--help"};
  int argc = 2;
  CHECK_THROWS_AS(
      fika::cli::parse_command_line(argc, const_cast<char **>(argv)),
      std::invalid_argument);
}
