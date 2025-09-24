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

#include "ipc_queue_manager.h"

#include "msg.h"

namespace fika::detail {

void boost_queue_manager::remove(const std::string &name) {
  boost::interprocess::message_queue::remove(name.c_str());
}

void boost_queue_manager::create(const std::string &name,
                                 std::size_t max_messages,
                                 std::size_t message_size) {
  auto mq = std::make_shared<boost::interprocess::message_queue>(
      boost::interprocess::create_only, name.c_str(), max_messages,
      message_size);
  queues_[name] = mq;  // keep handle alive
}
void boost_queue_manager::send_stop_job(const std::string &queue_name) {
  log::log_info("Sending stop job to {}", queue_name);
  ipc_message stop_job{poison_pill{}};
  queues_[queue_name]->send(&stop_job, sizeof(stop_job), 0);
  log::log_info("Sent stop job");
}

}  // namespace fika::detail