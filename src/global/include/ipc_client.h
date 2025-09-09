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
#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory>
#include <string>

namespace fika {

class ipc_client {
public:
  ipc_client(const std::string &qname);
  bool receive_job(file_job_shm &job);
  void send_result(const file_job_shm &job);

private:
  std::unique_ptr<boost::interprocess::message_queue> mq;
  std::unique_ptr<boost::interprocess::message_queue> mq_results;
};

} // namespace fika

#endif