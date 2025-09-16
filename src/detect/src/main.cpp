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

#include <detector.h>

#include <csignal>
#include <iostream>
#include <map>

#include "detectors/magic_api.h"
#include "file_job.h"
#include "ipc_client.h"
#include "mime_type.h"
#include "msg.h"
#include "service.h"

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting detect service");
  try {
    std::map<std::string,
             std::shared_ptr<fika::MsgQueueSender<fika::file_job_shm>>>
        senders;
    senders.emplace("qm",
                    std::make_shared<fika::MsgQueueSender<fika::file_job_shm>>(
                        "result_queue"));

    std::vector<std::unique_ptr<fika::file_detector>> dets;

    dets.push_back(std::make_unique<fika::magic_handle>(
        std::make_unique<fika::libmagic_api>()));

    fika::detect::detector d(std::move(dets));
    // The actual work to be done per job
    auto handler = [&d](const fika::file_job_shm &job)
        -> std::pair<std::string, fika::file_job_shm> {
      std::cout << "Processing job " << job.id << "\n";
      fika::file_job_shm r = job;
      auto result = d.detect_file(job.path);
      fika::mime::Type mime_info = fika::mime::map_type(result.mime_type);
      r.mime = mime_info;
      fika::log::log_info("Detect file: {}: {}", job.path, result.mime_type);
      if (result.mime_type != "application/octet-stream") {
        r.status = fika::Status::DETECTING;
      } else {
        r.status = fika::Status::FAILED;
      }
      return std::make_pair("qm", r);
    };

    fika::Service<fika::file_job_shm, fika::file_job_shm> service(
        std::make_unique<fika::MsgQueueReceiver<fika::file_job_shm>>(
            "job_queue_detector"),
        senders, handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    std::cerr << "Service failed: " << e.what() << std::endl;
    return 1;
  }

  fika::log::log_info("Stopping detect service");

  return 0;
}
