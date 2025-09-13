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

#include "magic_api.h"
#include "magic_handle.h"
#include <doctest/doctest.h>
#include <memory>
#include <stdexcept>

struct mock_magic_api : public fika::magic_api {
  void *open(int) override { return (void *)1; }
  int load(void *, const char *) override { return 0; }
  const char *file(void *, const char *path) override {
    if (std::string(path) == "bad")
      return nullptr;
    return "text/plain";
  }
  void close(void *) override {}
  const char *error(void *) override { return "mock error"; }
};

TEST_CASE("magic_handle file detection") {
  fika::magic_handle handle(std::make_unique<mock_magic_api>()); // inject mock

  CHECK(handle.detect("somefile") == "text/plain");
  CHECK_THROWS_AS(handle.detect("bad"), std::runtime_error);
}

TEST_CASE("magic_handle name() returns libmagic") {
  fika::magic_handle mh(std::make_unique<mock_magic_api>());
  CHECK(mh.name() == "libmagic");
}
