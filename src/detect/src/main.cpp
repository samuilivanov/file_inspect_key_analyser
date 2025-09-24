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

#include <detector.h>

#include <csignal>
#include <memory>

#include "detect_service.h"
#include "detectors/magic_api.h"
#include "ipc_client.h"
#include "msg.h"
#include "qn_rslv.h"
#include "service.h"

int main(int argc, char const *argv[]) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting detect service");
  try {
    std::vector<std::unique_ptr<fika::file_detector>> detectors;

    detectors.push_back(std::make_unique<fika::magic_handle>(
        std::make_unique<fika::libmagic_api>()));

    fika::detect::detector detect(std::move(detectors));
    // The actual work to be done per job

    std::unique_ptr<fika::Service> service =
        std::make_unique<fika::detect_service>(
            std::make_unique<fika::MsgQueueReceiver>(
                fika::util::get_direct_queue(
                    fika::util::make_sender("qm"),
                    fika::util::make_receiver("detect"))),
            std::move(detect));

    service->add_sender(
        "qm",
        std::make_shared<fika::MsgQueueSender>(
            fika::util::get_inbox_queue(fika::util::make_receiver("qm"))));

    service->start();

    service->stop();
  } catch (const std::exception &e) {
    fika::log::log_info("Service failed: {}", e.what());
    return 1;
  }

  fika::log::log_info("Stopping detect service");

  return 0;
}
