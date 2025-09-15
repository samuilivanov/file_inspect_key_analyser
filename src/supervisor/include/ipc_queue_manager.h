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

#ifndef SRC_SUPERVISOR_INCLUDE_IPC_QUEUE_MANAGER_H_
#define SRC_SUPERVISOR_INCLUDE_IPC_QUEUE_MANAGER_H_

// clang-format off
#include <map>
#include <string>
#include <memory>

#include "config.h"
#include "file_job.h"

#include <boost/interprocess/ipc/message_queue.hpp>
// clang-format on

namespace fika {

struct ipc_queue_manager {
  virtual ~ipc_queue_manager() = default;
  virtual void remove(const std::string &name) = 0;
  virtual void create(const std::string &name, std::size_t max_messages,
                      std::size_t message_size) = 0;
  virtual void send_stop_job(const std::string &queue_name) = 0;
};
namespace detail {

struct boost_queue_manager : public ipc_queue_manager {
  void remove(const std::string &name) override {
    boost::interprocess::message_queue::remove(name.c_str());
  }

  void create(const std::string &name, std::size_t max_messages,
              std::size_t message_size) override {
    auto mq = std::make_shared<boost::interprocess::message_queue>(
        boost::interprocess::create_only, name.c_str(), max_messages,
        message_size);
    queues_[name] = mq;  // keep handle alive
  }
  void send_stop_job(const std::string &queue_name) override {
    file_job_shm stop_job{};
    stop_job.stop = true;
    queues_[queue_name]->send(&stop_job, sizeof(stop_job), 0);
  }
  std::map<std::string, std::shared_ptr<boost::interprocess::message_queue>>
      queues_;
};
}  // namespace detail

}  // namespace fika

#endif  // SRC_SUPERVISOR_INCLUDE_IPC_QUEUE_MANAGER_H_
