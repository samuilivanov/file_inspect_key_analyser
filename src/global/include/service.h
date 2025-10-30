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

#ifndef SRC_GLOBAL_INCLUDE_SERVICE_H_
#define SRC_GLOBAL_INCLUDE_SERVICE_H_

// clang-format off
#include <atomic>
#include <functional>
#include <map>
#include <utility>
#include <memory>
#include <string>

#include "file_job.h"
#include "ipc_client.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
// clang-format on

namespace fika {

class Service {
 public:
  using handler_fn = std::function<bool(ipc_message)>;
  using JobHandler =
      std::function<std::pair<std::string, ipc_message>(const ipc_message&)>;

  Service(std::unique_ptr<queue_receiver> receiver,
          std::size_t threadCount = boost::thread::hardware_concurrency());
  Service(const Service&) = delete;
  Service(Service&&) = delete;
  Service& operator=(const Service&) = delete;
  Service& operator=(Service&&) = delete;
  virtual ~Service() { stop(); }
  Service() = delete;

  void start();

  void stop();

  virtual void handle(ipc_message msg) = 0;

  queue_sender* get_sender(const std::string& queue) {
    auto iter = senders_.find(queue);
    return iter != senders_.end() ? iter->second.get() : nullptr;
  }

  void add_sender(std::string queue, std::shared_ptr<queue_sender> sender) {
    senders_.emplace(std::move(queue), sender);
  }

 private:
  void receiveLoop();
  std::unique_ptr<queue_receiver> receiver_;
  std::map<std::string, std::shared_ptr<queue_sender>> senders_;
  boost::asio::thread_pool pool_;
  boost::thread receiverThread_;
  std::atomic<bool> running_;
};
}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_SERVICE_H_
