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

#include <memory>
#include <vector>

#include "ipc_queue_manager.h"
#include "queue_descriptor.h"
#include "supervisor.h"
#include "worker.h"

using namespace fika;

struct mock_child : public child_process {
  bool alive = true;
  bool terminated = false;

  bool running() const override { return alive; }
  void terminate() override {
    terminated = true;
    alive = false;
  }
  void wait() {}
  std::string name() const override { return "test"; }
};

struct mock_queue_manager : public ipc_queue_manager {
  std::vector<std::string> removed{};
  struct queue_info {
    std::string name{};
    size_t max{};
    size_t size{};
  };
  std::vector<queue_info> created{};

  void remove(const std::string &name) override { removed.push_back(name); }
  void create(const std::string &name, std::size_t max_messages,
              std::size_t message_size) override {
    created.push_back({name, max_messages, message_size});
  }
  void send_stop_job(const std::string &queue_name) {}
};

TEST_CASE("Supervisor creates workers correctly") {
  std::vector<mock_child *> mocks;  // raw pointers for assertions
  mock_queue_manager mock_mgr;
  std::vector<queue_descriptor> queues = {{"q1", 10, 128}, {"q2", 20, 256}};

  // Each factory produces a worker that produces a mock_child
  auto worker_factory = [&mocks]() -> std::unique_ptr<worker> {
    return std::make_unique<worker>(
        [&mocks]() -> std::unique_ptr<child_process> {
          auto m = std::make_unique<mock_child>();
          mocks.push_back(m.get());  // track the mock for assertions
          return m;                  // ownership transferred to worker
        });
  };

  supervisor sup(
      {worker_factory, worker_factory}, queues,
      std::make_shared<mock_queue_manager>(mock_mgr));  // create 2 workers
  CHECK(sup.get_workers().size() == 2);
  CHECK(mocks.empty());  // not started yet

  sup.start_workers();
  CHECK(mocks.size() == 2);  // children created
  for (auto &m : mocks) CHECK(m->running());
}

TEST_CASE("Supervisor monitor_once restarts dead workers") {
  std::vector<mock_child *> mocks;  // raw pointers for assertions
  mock_queue_manager mock_mgr;

  std::vector<queue_descriptor> queues = {{"q1", 10, 128}, {"q2", 20, 256}};
  // Each factory produces a worker that produces a mock_child
  auto worker_factory = [&mocks]() -> std::unique_ptr<worker> {
    return std::make_unique<worker>(
        [&mocks]() -> std::unique_ptr<child_process> {
          auto m = std::make_unique<mock_child>();
          mocks.push_back(m.get());  // track the mock for assertions
          return m;                  // ownership transferred to worker
        });
  };

  supervisor sup({worker_factory}, queues,
                 std::make_shared<mock_queue_manager>(mock_mgr));
  sup.start_workers();
  CHECK(mocks.size() == 1);

  // Simulate dead worker
  mocks[0]->alive = false;

  sup.monitor_once();  // restart dead workers

  // New mock created
  // TODO (samuil) not sure how to fix this
  CHECK(mocks.size() == 2);
  CHECK(mocks[1]->running());
  // CHECK(mocks[0]->terminated == false); // old mock not explicitly terminated
}

TEST_CASE("Supervisor multiple dead workers get restarted") {
  std::vector<mock_child *> mocks;  // raw pointers for assertions
  mock_queue_manager mock_mgr;

  std::vector<queue_descriptor> queues = {{"q1", 10, 128}, {"q2", 20, 256}};
  // Each factory produces a worker that produces a mock_child
  auto worker_factory = [&mocks]() -> std::unique_ptr<worker> {
    return std::make_unique<worker>(
        [&mocks]() -> std::unique_ptr<child_process> {
          auto m = std::make_unique<mock_child>();
          mocks.push_back(m.get());  // track the mock for assertions
          return m;                  // ownership transferred to worker
        });
  };

  supervisor sup({worker_factory, worker_factory, worker_factory}, queues,
                 std::make_shared<mock_queue_manager>(mock_mgr));
  sup.start_workers();
  CHECK(mocks.size() == 3);

  // Kill first two workers
  mocks[0]->alive = false;
  mocks[1]->alive = false;

  sup.monitor_once();
  CHECK(mocks.size() == 5);  // two new mocks created
  CHECK(mocks[3]->running());
  CHECK(mocks[4]->running());
}

TEST_CASE("Supervisor resets and creates queues") {
  std::shared_ptr<mock_queue_manager> mock_mgr =
      std::make_shared<mock_queue_manager>();

  std::vector<queue_descriptor> queues = {{"q1", 10, 128}, {"q2", 20, 256}};

  std::vector<supervisor::worker_factory_t> factories;  // empty for this test

  supervisor sup(factories, queues, mock_mgr);

  sup.reset_queues();
  CHECK(mock_mgr->removed.size() == 2);
  CHECK(mock_mgr->removed[0] == "q1");
  CHECK(mock_mgr->removed[1] == "q2");

  sup.create_queues();
  CHECK(mock_mgr->created.size() == 2);
  CHECK(mock_mgr->created[0].name == "q1");
  CHECK(mock_mgr->created[0].max == 10);
  CHECK(mock_mgr->created[0].size == 128);
  CHECK(mock_mgr->created[1].name == "q2");
}
