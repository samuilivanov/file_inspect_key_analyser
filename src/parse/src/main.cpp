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
#include <thread>

#include "ipc_client.h"
#include "msg.h"
#include "parser_registry.h"
#include "service.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
// clang-format on

namespace {
fika::file_job_shm process_job(const fika::file_job_shm &job,
                               fika::parser_registry *parsers) {
  fika::file_job_shm j = job;
  if (parsers->find_parser(job.mime)) {
    fika::log::log_info("in parsers");
    j.status = fika::Status::DONE;
  } else {
    j.status = fika::Status::FAILED;
  }
  return j;
}

}  // namespace

int main() {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting parser service");
  try {
    std::map<std::string,
             std::shared_ptr<fika::MsgQueueSender<fika::ipc_message>>>
        senders;
    senders.emplace("qm",
                    std::make_shared<fika::MsgQueueSender<fika::ipc_message>>(
                        "result_queue"));
    fika::parser_registry parsers;

    // The actual work to be done per job
    auto handler = [&parsers](const fika::ipc_message &msg)
        -> std::pair<std::string, fika::ipc_message> {
      std::cout << "Processing job " << msg.job.id << "\n";
      auto j = process_job(msg.job, &parsers);
      fika::ipc_message jj{j};
      return std::make_pair("qm", jj);
    };

    fika::Service<fika::ipc_message, fika::ipc_message> service(
        std::make_unique<fika::MsgQueueReceiver<fika::ipc_message>>(
            "job_queue_parse"),
        senders, handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    std::cerr << "Service failed: " << e.what() << std::endl;
    return 1;
  }
  fika::log::log_info("Stopping parse service");

  return 0;
}
