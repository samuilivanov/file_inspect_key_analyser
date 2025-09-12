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

#include "qipc.h"

namespace fika {
qipc::qipc(/* args */) {
  mq_detector = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_only, "job_queue_detector");
  mq_parse = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_only, "job_queue_parse");
  mq_result = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_only, "result_queue");
}

qipc::~qipc() {
}

void qipc::send_job(const file_job_shm &job) {
  switch (job.type) {
  case JobType::DETECTOR:
    mq_detector->send(&job, sizeof(job), 0);
    break;
  case JobType::PARSER:
    mq_parse->send(&job, sizeof(job), 0);

  default:
    break;
  }
}
void qipc::receive_result(file_job_shm &job) {
  size_t recv_size;
  unsigned int priority;
  mq_result->receive(&job, sizeof(job), recv_size, priority);
}
} // namespace fika
