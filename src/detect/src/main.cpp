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

#include "detectors/magic_api.h"
#include "file_job.h"
#include "ipc_client.h"
#include "msg.h"
#include "service.h"
#include <csignal>
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
  fika::log::msg_logger_init("detect.log");
  fika::log::log_info("Starting detect service");
  try {
    fika::MsgQueueSender<fika::file_job_shm> sender("result_queue");
    fika::MsgQueueReceiver<fika::file_job_shm> receiver("job_queue_detector");
    std::vector<std::unique_ptr<fika::file_detector>> dets;

    dets.push_back(std::make_unique<fika::magic_handle>(
        std::make_unique<fika::libmagic_api>()));

    fika::detect::detector d(std::move(dets));
    // The actual work to be done per job
    auto handler = [&d](const fika::file_job_shm &job) -> fika::file_job_shm {
      std::cout << "Processing job " << job.id << "\n";
      fika::file_job_shm r = job;
      auto result = d.detect_file(job.path);
      r.mime = mime_string_to_enum(result.mime_type);
      fika::log::log_info("Detect file: {}: {}", job.path, result.mime_type);
      if (result.mime_type != "application/octet-stream") {
        r.status = fika::Status::DETECTING;
      } else {
        r.status = fika::Status::FAILED;
      }
      return r;
    };

    fika::Service<fika::file_job_shm, fika::file_job_shm> service(
        receiver, sender, handler);

    service.start();

    std::cout << "Service running. Press Enter to stop...\n";
    std::cin.get();

    service.stop();

  } catch (const std::exception &e) {
    std::cerr << "Service failed: " << e.what() << std::endl;
    return 1;
  }

  fika::log::log_info("Stopping detect service");

  return 0;
}
