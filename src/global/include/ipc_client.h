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

#ifndef SRC_GLOBAL_INCLUDE_IPC_CLIENT_H_
#define SRC_GLOBAL_INCLUDE_IPC_CLIENT_H_

// clang-format off
#include <chrono>
#include <memory>
#include <optional>
#include <string>

#include "file_job.h"
#include "message_queues.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
// clang-format on

namespace fika {

template <typename Job>
class MsgQueueReceiver {
 public:
  explicit MsgQueueReceiver(const std::string &queue_name)
      : mq_recv_(boost::interprocess::open_only, queue_name.c_str()) {}
  void receive(Job &job) {
    size_t recv_size{};
    unsigned int priority{};
    mq_recv_.receive(&job, sizeof(Job), recv_size, priority);
  }

 private:
  boost::interprocess::message_queue mq_recv_;
};

template <typename Result>
class MsgQueueSender {
 public:
  explicit MsgQueueSender(const std::string &queue_name)
      : mq_send_(boost::interprocess::open_only, queue_name.c_str()) {}
  void send(const Result &result) { mq_send_.send(&result, sizeof(Result), 0); }

 private:
  boost::interprocess::message_queue mq_send_;
};

}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_IPC_CLIENT_H_
