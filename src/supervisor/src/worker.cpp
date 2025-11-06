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

#include "worker_configs.h"

namespace fika {

worker::worker(worker_config conf) : conf_(std::move(conf)) {}

void worker::start() {
  if (process_ && process_->running()) {
    return;
  }

  // build a fresh child_process from the spec
  process_ = std::make_unique<boost_child_process>(conf_.path, conf_.args);
}
void worker::stop() {
  if (process_) {
    process_->terminate();
  }
}

void worker::wait() const {
  if (process_) {
    process_->wait();
  }
}
bool worker::is_alive() const { return process_ && process_->running(); }

void worker::restart() {
  if (process_ && process_->running()) {
    process_->terminate();
  }
  start();
}

std::string worker::name() const { return conf_.name; }
[[nodiscard]] bool worker::start_on_boot() const { return conf_.start_on_boot; }

}  // namespace fika
