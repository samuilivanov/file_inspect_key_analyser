/*
 * This file is part of Fika.
 *
 * Copyright [2025] Samuil Ivanov
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

#include <boost/asio/steady_timer.hpp>

#include "config.h"
#include "config_loader.hpp"
#include "file_job.h"
#include "ipc_queue_manager.h"
#include "msg.h"
#include "queue_descriptor.h"
#include "supervisor.h"
#include "worker_configs.h"

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();

  std::vector<fika::queue_descriptor> queues = {
      {"job_queue_detector", 100, sizeof(fika::file_job_shm)},
      {"job_queue_parse", 100, sizeof(fika::file_job_shm)},
      {"result_queue", 100, sizeof(fika::file_job_shm)}};

  std::shared_ptr<fika::ipc_queue_manager> queue_mgr =
      std::make_shared<fika::detail::boost_queue_manager>();

  auto configs =
      fika::config::load<fika::worker_configs>(SUPERVISOR_CONF.data());

  std::vector<fika::supervisor::worker_factory_t> factories;
  for (auto &cfg : configs.workers) {
    factories.push_back([cfg]() -> std::unique_ptr<fika::worker> {
      return std::make_unique<fika::worker>([cfg]() {
        return std::make_unique<fika::boost_child_process>(cfg.path, cfg.args);
      });
    });

    std::cout << "Loaded worker: " << cfg.name << " (" << cfg.path << ")\n";
  }

  fika::supervisor sup(factories, queues, queue_mgr);
  sup.reset_queues();
  sup.create_queues();
  sup.register_commands();
  sup.run();

  return 0;
}
