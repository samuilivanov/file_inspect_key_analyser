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
#include <exception>

#include "config.h"
#include "config_loader.hpp"
#include "file_job.h"
#include "ipc_queue_manager.h"
#include "msg.h"
#include "qn_rslv.h"
#include "queue_descriptor.h"
#include "supervisor.h"
#include "worker_configs.h"

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting supervisor...");
  // TODO(samuil): move this to a separete function
  std::vector<fika::queue_descriptor> queues = {
      {fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                    fika::util::make_receiver("detect")),
       100, sizeof(fika::ipc_message)},
      {fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                    fika::util::make_receiver("parse")),
       100, sizeof(fika::ipc_message)},
      {fika::util::get_inbox_queue(fika::util::make_receiver("qm")), 100,
       sizeof(fika::ipc_message)}};

  std::shared_ptr<fika::ipc_queue_manager> queue_mgr =
      std::make_shared<fika::detail::boost_queue_manager>();

  auto configs =
      fika::config::load<fika::worker_configs>(SUPERVISOR_CONF.data());

  std::vector<fika::supervisor::worker_factory_t> factories;
  for (auto &cfg : configs.workers) {
    factories.emplace_back([cfg]() -> std::unique_ptr<fika::worker> {
      return std::make_unique<fika::worker>([cfg]() {
        return std::make_unique<fika::boost_child_process>(cfg.path, cfg.args);
      });
    });

    fika::log::log_info("Loaded worker: {} ({})", cfg.name, cfg.path);
  }
  try {
    fika::supervisor sup(factories, queues, queue_mgr);
    sup.reset_queues();
    sup.create_queues();
    sup.register_commands();
    sup.run();
  } catch (const std::exception &e) {
    fika::log::log_error("Exception in supervisor : {}", e.what());
  } catch (...) {
    fika::log::log_error("error");
  }

  return 0;
}
