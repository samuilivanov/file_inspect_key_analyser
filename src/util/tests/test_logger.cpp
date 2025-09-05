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
#include "doctest/doctest.h"
#include "msg.h"

TEST_CASE("bip39_ctx initializes and sets entropy") {
  msg_logger::msg_logger_init("test_logs.log");
  // Basic messages
  msg_logger::log_info("This is an info message");
  //   msg_debug("Debug value: x={}", 42);
  //   msg_error("An error occurred: code={}", -1);
}