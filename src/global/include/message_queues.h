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

#ifndef MESSAGE_QUEUES_H
#define MESSAGE_QUEUES_H

#include "file_job.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory>
#include <stddef.h>

namespace fika {

struct boost_job_sender {
  explicit boost_job_sender(const std::string &queue_name)
      : mq(boost::interprocess::open_only, queue_name.c_str()) {}

  void send(const file_job_shm &job);

private:
  boost::interprocess::message_queue mq;
};

struct boost_job_receiver {
  explicit boost_job_receiver(const std::string &queue_name)
      : mq(boost::interprocess::open_only, queue_name.c_str()) {}

  file_job_shm receive();

private:
  boost::interprocess::message_queue mq;
};
} // namespace fika

#endif