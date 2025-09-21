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

#include "service.h"

#include "msg.h"
namespace fika {

Service::Service(std::unique_ptr<queue_receiver> receiver,
                 std::map<std::string, std::shared_ptr<queue_sender>> senders,
                 JobHandler handler, std::size_t threadCount)
    : receiver_(std::move(receiver)),
      senders_(std::move(senders)),
      handler_(std::move(handler)),
      pool_(threadCount),
      running_(false) {}

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

    std::visit(
        [this](auto&& inner) {
          using T = std::decay_t<decltype(inner)>;

          if constexpr (std::is_same_v<T, file_job_shm>) {
            boost::asio::post(pool_, [this, job = inner] {
              auto [queue, result] = handler_(job);
              if (auto iter = senders_.find(queue); iter != senders_.end()) {
                log::log_info("sending to {} queue", queue);
                iter->second->send(result);
              } else {
                log::log_warn("no sender for queue {}", queue);
              }
            });
          } else if constexpr (std::is_same_v<T, poison_pill>) {
            log::log_info("received poison pill - stopping");
            running_ = false;
          }
        },
        msg);
  }
  pool_.join();
}
}  // namespace fika