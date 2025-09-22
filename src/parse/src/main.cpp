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

// clang-format off
#include "ipc_client.h"
#include "msg.h"
#include "parser_registry.h"
#include "service.h"
#include "qn_rslv.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
// clang-format on

namespace {
fika::file_job_shm process_job(const fika::file_job_shm &job,
                               fika::parser_registry *parsers) {
  fika::file_job_shm job_shm = job;
  if (parsers->find_parser(job.mime)) {
    fika::log::log_info("in parsers");
    job_shm.status = fika::Status::DONE;
  } else {
    job_shm.status = fika::Status::FAILED;
  }
  return job_shm;
}

}  // namespace

int main() {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting parser service");
  try {
    std::map<std::string, std::shared_ptr<fika::queue_sender>> senders;
    senders.emplace(
        "qm",
        std::make_shared<fika::MsgQueueSender>(
            fika::util::get_inbox_queue(fika::util::make_receiver("qm"))));
    fika::parser_registry parsers;

    // The actual work to be done per job
    auto handler = [&parsers](const fika::ipc_message &msg)
        -> std::pair<std::string, fika::ipc_message> {
      const auto &file_job = std::get<fika::file_job_shm>(msg);
      fika::log::log_info("Processing job {}",
                          std::string(file_job.job_id.data()));
      auto job_shm = process_job(file_job, &parsers);
      fika::ipc_message response_msg{job_shm};
      return std::make_pair("qm", response_msg);
    };

    fika::Service service(
        std::make_unique<fika::MsgQueueReceiver>(fika::util::get_direct_queue(
            fika::util::make_sender("qm"), fika::util::make_receiver("parse"))),
        senders, handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }
  fika::log::log_info("Stopping parse service");

  return 0;
}
