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

#include "ipc_client.h"

namespace fika {

ipc_client::ipc_client(const std::string &qname) {
  mq = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_only, "job_queue_detector");
  mq_results = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_only, "result_queue");
}

bool ipc_client::try_receive_job(file_job_shm &job) {
  size_t recv_size;
  unsigned int priority;
  return mq->try_receive(&job, sizeof(job), recv_size, priority);
}

void ipc_client::receive_job(file_job_shm &job) {
  size_t recv_size;
  unsigned int priority;
  mq->receive(&job, sizeof(job), recv_size, priority);
}

void ipc_client::send_result(const file_job_shm &job) {
  mq_results->send(&job, sizeof(job), 0);
}

} // namespace fika
