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
#include "../config_loader.hpp"
#include "test_config.h"
#include <doctest/doctest.h>

TEST_CASE("load testconf.json") {
  auto server = fika::config::load<test_config>(
      "/home/samuil/Projects/fika/conf/testconf.json"); // TODO (samuil) this
                                                        // should be in cmake as
                                                        // a solution

  CHECK(server.host == "127.0.0.1");
  CHECK(server.port == 9090);
}
