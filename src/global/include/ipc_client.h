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

#ifndef IPC_CLIENT_H
#define IPC_CLIENT_H
#include "file_job.h"
#include "message_queues.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <chrono>
#include <memory>
#include <optional>
#include <string>

namespace fika {

template <typename SendType, typename RecvType> class ipc_client {
public:
  ipc_client(const std::string &send_queue, const std::string &recv_queue) {
    // Only open existing queues
    mq_send_ = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_only, send_queue.c_str());
    mq_recv_ = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_only, recv_queue.c_str());
  }

  bool try_receive(RecvType &msg) {
    size_t recv_size;
    unsigned int priority;
    return mq_recv_->try_receive(&msg, sizeof(RecvType), recv_size, priority);
  }

  void receive_job(RecvType &msg) {
    size_t recv_size;
    unsigned int priority;
    mq_recv_->receive(&msg, sizeof(RecvType), recv_size, priority);
  }

  void send_result(const SendType &msg) {
    mq_send_->send(&msg, sizeof(SendType), 0);
  }

private:
  std::unique_ptr<boost::interprocess::message_queue> mq_recv_;
  std::unique_ptr<boost::interprocess::message_queue> mq_send_;
};

// Assume your Boost MQ wrappers
template <typename Job> class MsgQueueReceiver {
public:
  MsgQueueReceiver(const std::string &queue_name)
      : mq_recv_(boost::interprocess::open_only, queue_name.c_str()) {}
  void receive(Job &job) {
    size_t recv_size;
    unsigned int priority;
    mq_recv_.receive(&job, sizeof(Job), recv_size, priority);
  }

private:
  boost::interprocess::message_queue mq_recv_;
};

template <typename Result> class MsgQueueSender {
public:
  MsgQueueSender(const std::string &queue_name)
      : mq_send_(boost::interprocess::open_only, queue_name.c_str()) {}
  void send(const Result &result) {
    mq_send_.send(&result, sizeof(Result), 0);
  }

private:
  boost::interprocess::message_queue mq_send_;
};

} // namespace fika

#endif