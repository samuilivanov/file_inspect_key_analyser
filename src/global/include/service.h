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

#ifndef SRC_GLOBAL_INCLUDE_SERVICE_H_
#define SRC_GLOBAL_INCLUDE_SERVICE_H_

// clang-format off
#include <atomic>
#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <memory>
#include <string>

#include "ipc_client.h"
#include "msg.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
// clang-format on

namespace fika {

template <typename Job, typename Result>
class Service {
 public:
  using JobHandler = std::function<std::pair<std::string, Result>(const Job&)>;

  Service(
      std::unique_ptr<MsgQueueReceiver<Job>> receiver,
      std::map<std::string, std::shared_ptr<MsgQueueSender<Result>>> senders,
      JobHandler handler,
      std::size_t threadCount = boost::thread::hardware_concurrency())
      : receiver_(std::move(receiver)),
        senders_(std::move(senders)),
        handler_(std::move(handler)),
        pool_(threadCount),
        running_(false) {}
  Service(const Service&) = delete;
  Service(Service&&) = delete;
  Service& operator=(const Service&) = delete;
  Service& operator=(Service&&) = delete;
  ~Service() { stop(); }

  void start() {
    running_ = true;
    receiveLoop();
  }

  void stop() {
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

 private:
  void receiveLoop() {
    while (running_) {
      Job msg{};
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
            } else if constexpr (std::is_same_v<T, int>) {
              log::log_info("received poison pill - stopping");
              running_ = false;
            }
          },
          msg);
    }
    pool_.join();
  }

  std::unique_ptr<MsgQueueReceiver<Job>> receiver_;
  std::map<std::string, std::shared_ptr<MsgQueueSender<Result>>> senders_;
  JobHandler handler_;
  boost::asio::thread_pool pool_;
  boost::thread receiverThread_;
  std::atomic<bool> running_;
};
}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_SERVICE_H_
