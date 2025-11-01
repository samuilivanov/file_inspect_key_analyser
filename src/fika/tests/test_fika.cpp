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

#include <span>

#include "cli.h"
#include "cli_parser.h"

TEST_CASE("parse start command with service") {
  char *argv[] = {"fika", "-c", "start", "-s", "parser"};
  int argc = 5;

  fika::cli_parser cli(argc, argv);
  cli.initialize(fika::default_option::help | fika::default_option::version);
  cli.set_options_description(fika::cli::command_description());
  auto vm = cli.parse();

  auto result = fika::cli::parse_command_line(vm.value());
  CHECK(result.type == fika::CommandType::Start);
  CHECK(result.service == "parser");
}

TEST_CASE("parse stop command without service") {
  char *argv[] = {"fika", "-c", "stop"};
  int argc = 3;
  fika::cli_parser cli(argc, argv);
  cli.initialize(fika::default_option::help | fika::default_option::version);
  cli.set_options_description(fika::cli::command_description());
  auto vm = cli.parse();
  auto result = fika::cli::parse_command_line(vm.value());
  CHECK(result.type == fika::CommandType::Stop);
  CHECK(result.service.empty());
}