/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "worker.h"

namespace fika {

worker::worker(process_factory_t factory) : factory_(std::move(factory)) {}

void worker::start() {
  process_ = factory_();
  last_heartbeat_ = std::chrono::steady_clock::now();
}
void worker::stop() {
  if (process_) process_->terminate();
}

void worker::wait() const {
  if (process_) process_->wait();
}
bool worker::is_alive() const { return process_ && process_->running(); }

void worker::restart() {
  if (process_ && process_->running()) {
    process_->terminate();
  }
  start();
}

std::string worker::name() const { return process_->name(); }

}  // namespace fika
