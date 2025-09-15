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
#include "qm.h"
#include "service.h"
#include <file_job.h>
#include <thread>

int main(int argc, char const *argv[]) {

  fika::log::msg_logger_init("qm.log");
  fika::log::log_info("Starting qm");
  fika::MsgQueueReceiver<fika::file_job_shm> receiver("result_queue");

  std::map<std::string,
           std::shared_ptr<fika::MsgQueueSender<fika::file_job_shm>>>
      senders;
  senders.emplace("parse",
                  std::make_shared<fika::MsgQueueSender<fika::file_job_shm>>(
                      "job_queue_parse"));
  senders.emplace("detect",
                  std::make_shared<fika::MsgQueueSender<fika::file_job_shm>>(
                      "job_queue_detector"));

  fika::qm q;
  q.setup();

  auto handler = [&q](const fika::file_job_shm &job)
      -> std::pair<std::string, fika::file_job_shm> {
    return q.process_results(job);
  };

  fika::Service<fika::file_job_shm, fika::file_job_shm> service(
      receiver, senders, handler);

  service.start();

  service.stop();

  return 0;
}
