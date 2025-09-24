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

#include "file_job.h"
#include "ipc_client.h"
#include "msg.h"
#include "pickup.h"
#include "qn_rslv.h"
#include "service.h"
int main(int argc, char **argv) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting pickup service");
  try {
    std::map<std::string, std::shared_ptr<fika::queue_sender>> senders;
    senders.emplace(
        "qm",
        std::make_shared<fika::MsgQueueSender>(
            fika::util::get_inbox_queue(fika::util::make_receiver("qm"))));

    // The actual work to be done per job
    auto handler = [&](const fika::ipc_message &msg)
        -> std::pair<std::string, fika::ipc_message> {
      return {"pickup", msg};
    };

    fika::Service service(
        std::make_unique<fika::MsgQueueReceiver>(
            fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                         fika::util::make_receiver("pickup"))),
        senders, handler);

    service.start();

    service.stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }
  fika::log::log_info("Stopping pickup service");
  return 0;
}