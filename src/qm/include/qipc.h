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

#ifndef QIPC_H
#define QIPC_H
#include "file_job.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory>
namespace fika {

class qipc {
public:
  qipc(/* args */);
  ~qipc();
  void send_job(const file_job_shm &job);
  bool try_receive_result(file_job_shm &job);

private:
  std::unique_ptr<boost::interprocess::message_queue> mq_detector;
  std::unique_ptr<boost::interprocess::message_queue> mq_result;
};

} // namespace fika

#endif