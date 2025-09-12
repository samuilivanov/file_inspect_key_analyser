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

#ifndef SUPERVISON_H
#define SUPERVISON_H

#include "ipc_queue_manager.h"
#include "queue_descriptor.h"
#include "worker.h"
#include <memory>
#include <vector>

namespace fika {

class supervisor {
public:
  using worker_factory_t = std::function<std::unique_ptr<worker>()>;

  explicit supervisor(std::vector<worker_factory_t> factories,
                      std::vector<fika::queue_descriptor> queues,
                      std::shared_ptr<ipc_queue_manager> queue_mgr);
  void run();

  template <typename Timer> void run_monitor_loop(Timer &timer) {
    monitor_once();
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([this, &timer](auto) { run_monitor_loop(timer); });
  }
  void start_workers();

  void reset_queues();
  void create_queues();
  std::vector<queue_descriptor> queues_;

  const std::vector<std::unique_ptr<worker>> &get_workers() const {
    return workers_;
  }

  void monitor_once();

private:
  std::vector<std::unique_ptr<worker>> workers_;
  std::shared_ptr<ipc_queue_manager> queue_mgr_;
};

} // namespace fika

#endif