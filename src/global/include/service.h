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

#ifndef SERVICE_H_INCLUDE
#define SERVICE_H_INCLUDE

#include "ipc_client.h"
#include <atomic>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <functional>
#include <iostream>

namespace fika {

struct StopJob {}; // special type

template <typename Job, typename Result> class Service {
public:
  using JobHandler = std::function<Result(const Job &)>;

  Service(MsgQueueReceiver<Job> &receiver, MsgQueueSender<Result> &sender,
          JobHandler handler,
          std::size_t threadCount = boost::thread::hardware_concurrency())
      : receiver_(receiver), sender_(sender), handler_(std::move(handler)),
        pool_(threadCount), running_(false) {}

  ~Service() { stop(); }

  void start() {
    running_ = true;
    receiverThread_ = boost::thread([this] { receiveLoop(); });
  }

  void stop() {
    if (!running_)
      return;
    running_ = false;
    pool_.join(); // wait for pool tasks
    receiverThread_.join();
  }

private:
  void receiveLoop() {
    while (running_) {
      Job job;
      receiver_.receive(job);
      // Detect poison pill
      if constexpr (std::is_same_v<Job, StopJob>) {
        if (!running_)
          break;
      }
      boost::asio::post(pool_, [this, job] {
        Result result = handler_(job);
        sender_.send(result);
      });
    }
  }

  MsgQueueReceiver<Job> &receiver_;
  MsgQueueSender<Result> &sender_;
  JobHandler handler_;
  boost::asio::thread_pool pool_;
  boost::thread receiverThread_;
  std::atomic<bool> running_;
};
} // namespace fika

#endif
