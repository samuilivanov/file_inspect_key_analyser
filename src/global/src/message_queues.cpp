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

#include "message_queues.h"

namespace fika {
void boost_job_sender::send(const file_job_shm &job) {
  mq.send(&job, sizeof(job), 0);
}

file_job_shm boost_job_receiver::receive() {
  file_job_shm job{};
  size_t recv_size{};
  unsigned int priority{};
  mq.receive(&job, sizeof(job), recv_size, priority);
  return job;
}
}  // namespace fika
