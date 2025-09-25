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
#include <string>

#include "file_job.h"
#include "msg.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
// clang-format on

namespace fika {

struct queue_receiver {
  virtual ~queue_receiver() = default;
  queue_receiver() = default;
  queue_receiver(const queue_receiver &) = delete;
  queue_receiver(queue_receiver &&) = delete;
  queue_receiver &operator=(const queue_receiver &) = delete;
  queue_receiver &operator=(queue_receiver &&) = delete;
  virtual void receive(ipc_message &job) = 0;
};

struct queue_sender {
  virtual ~queue_sender() = default;
  queue_sender() = default;
  queue_sender(const queue_sender &) = delete;
  queue_sender(queue_sender &&) = delete;
  queue_sender &operator=(const queue_sender &) = delete;
  queue_sender &operator=(queue_sender &&) = delete;
  virtual void send(const ipc_message &job) = 0;
};

class MsgQueueReceiver : public queue_receiver {
 public:
  explicit MsgQueueReceiver(const std::string &queue_name)
      : mq_recv_(boost::interprocess::open_only, queue_name.c_str()) {}
  void receive(ipc_message &job) override;

 private:
  boost::interprocess::message_queue mq_recv_;
};

class MsgQueueSender : public queue_sender {
 public:
  explicit MsgQueueSender(const std::string &queue_name)
      : mq_send_(boost::interprocess::open_only, queue_name.c_str()) {}
  void send(const ipc_message &result) override;

 private:
  boost::interprocess::message_queue mq_send_;
};

class null_sender : public queue_sender {
 public:
  void send(const ipc_message &result) override {
    log::log_debug("Send to null sender");
  }
};

}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_IPC_CLIENT_H_
