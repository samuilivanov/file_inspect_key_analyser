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
#include "ipc_client.h"
#include "msg.h"
#include "parser_registry.h"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>

namespace {
fika::file_job_shm process_job(const fika::file_job_shm &job,
                               fika::parser_registry &parsers) {
  fika::file_job_shm j = job;
  if (auto parser = parsers.find_parser(job.mime)) {
    fika::log::log_info("in parsers");
    j.status = fika::Status::DONE;
  } else {
    j.status = fika::Status::FAILED;
  }
  return j;
}

} // namespace

int main() {
  fika::log::msg_logger_init("parser.log");

  fika::log::log_info("Starting parser service");

  fika::ipc_client<fika::file_job_shm, fika::file_job_shm> ipc(
      "result_queue", "job_queue_parse");
  boost::asio::thread_pool pool_{std::thread::hardware_concurrency()};

  fika::parser_registry parsers;

  while (true) {
    fika::file_job_shm job{};
    ipc.receive_job(job);
    fika::log::log_info("receive job id: {}", job.id);
    boost::asio::post(pool_, [job_ = job, &parsers, &ipc] {
      auto result = process_job(job_, parsers);
      ipc.send_result(result);
    });
  }

  return 0;
}
