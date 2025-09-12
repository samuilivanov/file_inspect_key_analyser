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

#include "supervisor.h"
#include "config.h"
#include "config_loader.hpp"
#include "msg.h"
#include "worker_configs.h"
#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

namespace fika {

supervisor::supervisor(std::vector<worker_factory_t> factories,
                       std::vector<queue_descriptor> queues,
                       std::shared_ptr<ipc_queue_manager> queue_mgr)
    : queues_(queues), queue_mgr_(queue_mgr) {
  for (auto &f : factories)
    workers_.push_back(f());
}

void supervisor::start_workers() {
  for (auto &w : workers_) {
    w->start();
  }
}

void supervisor::monitor_once() {
  for (auto &w : workers_) {
    if (!w->is_alive()) {
      log::log_info("Restarting worker");
      w->restart();
    }
  }
}

void supervisor::reset_queues() {
  for (const auto &q : queues_) {
    queue_mgr_->remove(q.name);
    log::log_info("Removed stale queue: {}", q.name);
  }
}

void supervisor::create_queues() {
  for (const auto &q : queues_) {
    queue_mgr_->create(q.name, q.max_messages, q.message_size);
    std::cout << "Created queue: " << q.name
              << " (max_messages=" << q.max_messages
              << ", message_size=" << q.message_size << ")\n";
  }
}

} // namespace fika
