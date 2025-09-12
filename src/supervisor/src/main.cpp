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

#include "config.h"
#include "config_loader.hpp"
#include "msg.h"
#include "supervisor.h"
#include "worker_configs.h"
#include <boost/asio/steady_timer.hpp>

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();

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

  fika::supervisor sup(factories);

  sup.start_workers();

  boost::asio::io_context io;
  boost::asio::steady_timer timer(io, std::chrono::seconds(1));

  sup.run_monitor_loop(timer);

  io.run();

  return 0;
}
