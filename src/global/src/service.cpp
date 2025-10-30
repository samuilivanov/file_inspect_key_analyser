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

#include "service.h"

#include "msg.h"
namespace fika {

Service::Service(std::unique_ptr<queue_receiver> receiver,
                 std::size_t threadCount)
    : receiver_(std::move(receiver)), pool_(threadCount), running_(false) {}

void Service::start() {
  running_ = true;
  receiveLoop();
}

void Service::stop() {
  if (!running_) {
    return;
  }
  running_ = false;
  if (receiverThread_.joinable()) {
    try {
      receiverThread_.join();
    } catch (const std::exception& e) {
      log::log_error("Error while joining thread: {}", e.what());
    }
  }
}

void Service::receiveLoop() {
  while (running_) {
    ipc_message msg{};
    receiver_->receive(msg);
    if (std::holds_alternative<poison_pill>(msg)) {
      log::log_info("received poison pill - stopping");
      running_ = false;
    } else {
      boost::asio::post(pool_, [this, msg] {
        try {
          handle(msg);
        } catch (const std::exception& e) {
          log::log_error("Worker exception: {}", e.what());
        } catch (...) {
          log::log_error("Unknown worker exception");
        }
      });
    }
  }
  pool_.join();
}
}  // namespace fika
