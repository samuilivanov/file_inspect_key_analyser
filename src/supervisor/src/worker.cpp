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

#include "worker.h"

namespace fika {

worker::worker(process_factory_t factory) : factory_(std::move(factory)) {}

void worker::start() {
  process_ = factory_();
  last_heartbeat_ = std::chrono::steady_clock::now();
}
bool worker::is_alive() const { return process_ && process_->running(); }

void worker::restart() {
  if (process_ && process_->running()) {
    process_->terminate();
    process_->wait();
  }
  start();
}

} // namespace fika
