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

#include <memory>

#include "ipc_client.h"
#include "msg.h"
#include "qm.h"
#include "qm_service.h"
#include "qn_rslv.h"
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
    fika::qm q;
    setup();

    std::unique_ptr<fika::Service> service = std::make_unique<fika::qm_service>(
        std::make_unique<fika::MsgQueueReceiver>(
            fika::util::get_inbox_queue(fika::util::make_receiver("qm"))),
        q);
    service->add_sender("parse", std::make_shared<fika::MsgQueueSender>(
                                     fika::util::get_direct_queue(
                                         fika::util::make_sender("qm"),
                                         fika::util::make_receiver("parse"))));
    service->add_sender(
        "detect",
        std::make_shared<fika::MsgQueueSender>(
            fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                         fika::util::make_receiver("detect"))));
    service->start();

    service->stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }

  return 0;
}
