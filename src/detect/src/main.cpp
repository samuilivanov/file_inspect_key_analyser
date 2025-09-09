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

#include "file_job.h"
#include "ipc_client.h"
#include "msg.h"
#include <detector.h>
#include <iostream>

int main(int argc, char const *argv[]) {
  msg_logger::msg_logger_init("detect.log");
  fika::ipc_client ipc("job_queue_detector");

  while (true) {
    fika::file_job_shm job{};
    if (ipc.receive_job(job)) {
      msg_logger::log_info("processing job {}", job.id);
      fika::detect::detector d;
      auto result = d.detect_file(job.path);
      msg_logger::log_info("Detect file: {}: {}", job.path, result.mime_type);
      if (result.mime_type != "application/octet-stream") {
        job.status = fika::Status::PARSING;
      } else {
        job.status = fika::Status::FAILED;
      }
      ipc.send_result(job);
    }
  }

  return 0;
}
