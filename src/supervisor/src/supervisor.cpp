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
#include "config_loader.hpp"
#include "worker_configs.h"
#include <boost/asio.hpp>

namespace fika {

supervisor::supervisor() {
  auto configs = config::load<worker_configs>(
      "/home/samuil/Projects/fika/conf/supervisor.json");

  for (auto &cfg : configs.workers) {
    workers_.push_back(std::make_unique<worker>(cfg.path, cfg.args));
    std::cout << "Loaded worker: " << cfg.name << " (" << cfg.path << ")\n";
  }
}

void supervisor::start_workers() {
  for (auto &w : workers_) {
    w->start();
  }
}

void supervisor::monitor_workers() {
  boost::asio::io_context io;
  boost::asio::steady_timer timer(io, std::chrono::seconds(1));

  timer.async_wait([this, &timer](auto) {
    for (auto &w : workers_) {
      if (!w->is_alive()) {
        std::cout << "Restarting worker\n";
        w->restart();
      }
    }
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([this, &timer](auto) { monitor_workers(); });
  });

  io.run();
}

void supervisor::run() {
  start_workers();
  monitor_workers();
}

} // namespace fika
