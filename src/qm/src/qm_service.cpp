/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "qm_service.h"

#include "msg.h"

namespace fika {
qm_service::qm_service(std::unique_ptr<queue_receiver> receiver,
                       qm queue_manager)
    : Service(std::move(receiver)), queue_manager_(std::move(queue_manager)) {}
void qm_service::handle(ipc_message msg) {
  std::visit(
      [this](auto&& inner) {
        using T = std::decay_t<decltype(inner)>;

        if constexpr (std::is_same_v<T, file_job_shm>) {
          const auto& file_job = inner;

          fika::log::log_info("Processing job {}",
                              std::string(file_job.job_id.data()));
          fika::file_job_shm job_from_msg = file_job;

          auto r = queue_manager_.process_results(job_from_msg);
          auto* sender = get_sender(r.first);
          if (sender != nullptr) {
            sender->send(r.second);
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