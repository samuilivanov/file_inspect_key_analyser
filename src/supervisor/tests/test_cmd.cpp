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
#include <doctest/doctest.h>

#include "cmd_def.h"
#include "commands.h"

struct test_supervisor : public fika::supervisor {
  std::vector<std::string> started;
  std::vector<std::string> stopped;
  std::vector<fika::CommandResponse> sent_messages;

  test_supervisor(std::vector<worker_factory_t> factories,
                  std::vector<fika::queue_descriptor> queues,
                  std::shared_ptr<fika::ipc_queue_manager> queue_mgr)
      : supervisor(factories, queues, queue_mgr) {}

  void start_workers(const std::string &service_name = "") override {
    started.push_back(service_name);
  }

  void stop_workers(const std::string &service_name = "") override {
    stopped.push_back(service_name);
  }
  void send_pong(const fika::CommandResponse &msg) override {
    sent_messages.push_back(msg);
  }
};

TEST_CASE("start_command executes start_workers") {
  std::vector<std::function<std::unique_ptr<fika::worker>()>> factories;
  std::vector<fika::queue_descriptor> queues;
  std::shared_ptr<fika::ipc_queue_manager> queue_mgr;
  test_supervisor sup(factories, queues, queue_mgr);
  fika::detail::start_command cmd;

  cmd.execute(&sup, "parser");

  CHECK(sup.started.size() == 1);
  CHECK(sup.started[0] == "parser");
  CHECK(sup.stopped.empty());
}

TEST_CASE("stop_command executes stop_workers") {
  std::vector<std::function<std::unique_ptr<fika::worker>()>> factories;
  std::vector<fika::queue_descriptor> queues;
  std::shared_ptr<fika::ipc_queue_manager> queue_mgr;
  test_supervisor sup(factories, queues, queue_mgr);
  fika::detail::stop_command cmd;

  cmd.execute(&sup, "parser");

  CHECK(sup.stopped.size() == 1);
  CHECK(sup.stopped[0] == "parser");
  CHECK(sup.started.empty());
}

TEST_CASE("restart_command stops and starts workers") {
  std::vector<std::function<std::unique_ptr<fika::worker>()>> factories;
  std::vector<fika::queue_descriptor> queues;
  std::shared_ptr<fika::ipc_queue_manager> queue_mgr;
  test_supervisor sup(factories, queues, queue_mgr);
  fika::detail::restart_command cmd;

  cmd.execute(&sup, "parser");

  CHECK(sup.stopped.size() == 1);
  CHECK(sup.stopped[0] == "parser");

  CHECK(sup.started.size() == 1);
  CHECK(sup.started[0] == "parser");
}

TEST_CASE("ping_command executes send_pong with pong message") {
  std::vector<std::function<std::unique_ptr<fika::worker>()>> factories;
  std::vector<fika::queue_descriptor> queues;
  std::shared_ptr<fika::ipc_queue_manager> queue_mgr;
  test_supervisor sup(factories, queues, queue_mgr);

  fika::detail::ping_command cmd;

  cmd.execute(&sup, "any_service");

  REQUIRE(sup.sent_messages.size() == 1);
  CHECK(sup.sent_messages[0].success == true);
  CHECK(std::string(sup.sent_messages[0].message.data()) == "pong");
}