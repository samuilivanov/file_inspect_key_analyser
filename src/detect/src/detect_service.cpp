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

#include "detect_service.h"

#include "detector.h"
#include "msg.h"
#include "service.h"

namespace fika {
detect_service::detect_service(std::unique_ptr<queue_receiver> receiver,
                               detect::detector detect)
    : Service(std::move(receiver)), detect_(std::move(detect)) {}

void detect_service::handle(ipc_message msg) {
  std::visit(
      [this](auto&& inner) {
        using T = std::decay_t<decltype(inner)>;

        if constexpr (std::is_same_v<T, file_job_shm>) {
          const auto& file_job = inner;

          fika::log::log_info("Processing job {}",
                              std::string(file_job.job_id.data()));
          fika::file_job_shm job_from_msg = file_job;
          auto result = detect_.detect_file(std::string(file_job.path.data()));
          fika::mime::Type mime_info = fika::mime::map_type(result.mime_type);
          job_from_msg.mime = mime_info;
          fika::log::log_info("Detect file: {}: {}",
                              std::string(file_job.path.data()),
                              result.mime_type);
          if (result.mime_type != "application/octet-stream") {
            job_from_msg.status = fika::Status::DETECTING;
          } else {
            job_from_msg.status = fika::Status::FAILED;
          }
          fika::ipc_message res{job_from_msg};
          auto* sender = get_sender("qm");
          if (sender != nullptr) {
            sender->send(res);
          } else {
            log::log_warn("no sender for queue {}", "qm");
          }

        } else {
          log::log_warn("unknown command");
        }
      },
      msg);
}
}  // namespace fika
