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

#include <file_job.h>

#include <thread>

#include "ipc_client.h"
#include "msg.h"
#include "qm.h"
#include "service.h"

namespace {
void setup() {
  // TODO(samuil): redesign this setup function
  std::filesystem::path root = FIKA_SPOOL_DIR.data();
  fika::log::log_debug("Setup starting using path: {}", root.string());
  std::array<std::string, 5> subdirs{"incomming", "new", "processing", "done",
                                     "failed"};
  for (const auto &subdir : subdirs) {
    std::filesystem::path dir = root / subdir;
    if (std::filesystem::create_directories(dir)) {
      fika::log::log_info(std::string("created dir: ") + dir.string());
    }
  }
}

}  // namespace

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting qm");
  try {
    /* code */
    std::map<std::string, std::shared_ptr<fika::queue_sender>> senders;
    senders.emplace("parse",
                    std::make_shared<fika::MsgQueueSender>("job_queue_parse"));
    senders.emplace(
        "detect", std::make_shared<fika::MsgQueueSender>("job_queue_detector"));

    fika::qm q;
    setup();

    auto handler = [&q](const fika::ipc_message &msg)
        -> std::pair<std::string, fika::ipc_message> {
      // fika::log::log_info("mgs type: {}, msg id: {}", msg.type, msg.job.id);
      const auto &file_job = std::get<fika::file_job_shm>(msg);
      auto r = q.process_results(file_job);
      fika::ipc_message msg_res{r.second};
      return std::make_pair(r.first, msg_res);
    };

    fika::Service service(
        std::make_unique<fika::MsgQueueReceiver>("result_queue"), senders,
        handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }

  return 0;
}
