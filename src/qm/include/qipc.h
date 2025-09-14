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

template <typename detector_t, typename parser_t, typename result_t>
struct qipc {
  detector_t mq_detector;
  parser_t mq_parser;
  result_t mq_result;
  void send_job(const file_job_shm &job) {
    switch (job.type) {
    case JobType::DETECTOR:
      mq_detector.send(job);
      break;
    case JobType::PARSER:
      mq_parser.send(job);
      break;
    default:
      break;
    }
  }

  file_job_shm receive_result() { return mq_result.receive(); }
};

} // namespace fika

#endif