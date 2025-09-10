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

// TODO (samuil) this function should be moved to another location it will
// become quite large inless something else if thought of
namespace {

inline fika::MimeType mime_string_to_enum(const std::string &mimeStr) {
  if (mimeStr == "application/pdf")
    return fika::MimeType::PDF;
  if (mimeStr == "text/plain")
    return fika::MimeType::TEXT;
  if (mimeStr == "image/jpeg")
    return fika::MimeType::IMAGE_PNG;
  return fika::MimeType::UNKNOWN;
}

} // namespace

int main(int argc, char const *argv[]) {
  msg_logger::msg_logger_init("detect.log");
  fika::ipc_client ipc("job_queue_detector");

  // TODO (samuil) in general this logic can be moved to a separete class
  while (true) {
    fika::file_job_shm job{};
    ipc.receive_job(job);
    msg_logger::log_info("processing job {}", job.id);
    fika::detect::detector d;
    auto result = d.detect_file(job.path);
    job.mime = mime_string_to_enum(result.mime_type);
    msg_logger::log_info("Detect file: {}: {}", job.path, result.mime_type);
    if (result.mime_type != "application/octet-stream") {
      job.status = fika::Status::DETECTING;
    } else {
      job.status = fika::Status::FAILED;
    }
    ipc.send_result(job);
  }

  return 0;
}
