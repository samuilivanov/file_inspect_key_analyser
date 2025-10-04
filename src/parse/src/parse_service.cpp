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

#include "parse_service.h"

#include "file_job.h"
#include "msg.h"
#include "service.h"

namespace {
fika::file_job_shm process_job(const fika::file_job_shm& job,
                               fika::parser_registry* parsers) {
  fika::file_job_shm job_shm = job;
  // TODO(samuil): find parser should not be used and just prep the message
  // queue and send to the designated parser this will be a binary so message
  // queues will be used
  if (parsers->find_parser(job.mime)) {
    fika::log::log_info("in parsers");
    job_shm.status = fika::Status::DONE;
  } else {
    job_shm.status = fika::Status::FAILED;
  }
  return job_shm;
}

}  // namespace

namespace fika {

parse_service::parse_service(std::unique_ptr<queue_receiver> receiver,
                             fika::parser_registry parsers)
    : Service(std::move(receiver)), parsers_(std::move(parsers)) {}
void parse_service::handle(ipc_message msg) {
  std::visit(
      [this](auto&& inner) {
        using T = std::decay_t<decltype(inner)>;

        if constexpr (std::is_same_v<T, file_job_shm>) {
          const auto& file_job = inner;

          fika::log::log_info("Processing job {}",
                              std::string(file_job.job_id.data()));
          fika::file_job_shm job_from_msg = file_job;

          auto job_shm = process_job(file_job, &parsers_);
          fika::ipc_message response_msg{job_shm};
          auto* sender = get_sender("qm");
          if (sender != nullptr) {
            sender->send(response_msg);
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