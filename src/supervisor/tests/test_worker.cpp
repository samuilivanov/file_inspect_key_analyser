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
#include "worker.h"
#include "worker_configs.h"
#include <doctest/doctest.h>

struct mock_child : public fika::child_process {
  bool alive = true;
  bool terminate_called = false;

  bool running() const override { return alive; }
  void terminate() override {
    terminate_called = true;
    alive = false;
  }
  void wait() {}
};

TEST_CASE("worker start and is_alive") {
  fika::worker_config cfg{"test_bin", "path/to/exe", {"arg1", "arg2"}};

  auto lambda = [&cfg]() -> std::unique_ptr<fika::child_process> {
    return std::make_unique<mock_child>();
  };

  fika::worker w(lambda);

  CHECK(!w.is_alive());
  w.start();
  CHECK(w.is_alive());
}

TEST_CASE("worker restart terminates old child") {
  fika::worker_config cfg{"test_bin", "path/to/exe", {"arg1", "arg2"}};

  auto lambda = [&cfg]() -> std::unique_ptr<fika::child_process> {
    return std::make_unique<mock_child>();
  };
  fika::worker w(lambda);

  w.start();
  CHECK(w.is_alive());
  w.restart();
  CHECK(w.is_alive());
}
