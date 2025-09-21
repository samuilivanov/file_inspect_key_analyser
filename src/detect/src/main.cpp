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
    std::map<std::string, std::shared_ptr<fika::queue_sender>> senders;
    senders.emplace("qm",
                    std::make_shared<fika::MsgQueueSender>("result_queue"));

    std::vector<std::unique_ptr<fika::file_detector>> detectors;

    detectors.push_back(std::make_unique<fika::magic_handle>(
        std::make_unique<fika::libmagic_api>()));

    fika::detect::detector detect(std::move(detectors));
    // The actual work to be done per job
    auto handler = [&detect](const fika::ipc_message &msg)
        -> std::pair<std::string, fika::ipc_message> {
      const auto &file_job = std::get<fika::file_job_shm>(msg);

      fika::log::log_info("Processing job {}",
                          std::string(file_job.job_id.data()));
      fika::file_job_shm job_from_msg = file_job;
      auto result = detect.detect_file(std::string(file_job.path.data()));
      fika::mime::Type mime_info = fika::mime::map_type(result.mime_type);
      job_from_msg.mime = mime_info;
      fika::log::log_info("Detect file: {}: {}",
                          std::string(file_job.path.data()), result.mime_type);
      if (result.mime_type != "application/octet-stream") {
        job_from_msg.status = fika::Status::DETECTING;
      } else {
        job_from_msg.status = fika::Status::FAILED;
      }
      fika::ipc_message res{job_from_msg};
      return std::make_pair("qm", res);
    };

    fika::Service service(
        std::make_unique<fika::MsgQueueReceiver>("job_queue_detector"), senders,
        handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }

  fika::log::log_info("Stopping detect service");

  return 0;
}
