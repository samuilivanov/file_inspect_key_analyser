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

namespace fika {

supervisor::supervisor(std::vector<worker_factory_t> factories) {
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

} // namespace fika
