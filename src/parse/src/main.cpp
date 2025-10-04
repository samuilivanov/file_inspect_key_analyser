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
#include "parse_service.h"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <memory>
// clang-format on

int main() {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting parser service");
  try {
    fika::parser_registry parsers;

    std::unique_ptr<fika::Service> service =
        std::make_unique<fika::parse_service>(
            std::make_unique<fika::MsgQueueReceiver>(
                fika::util::get_direct_queue(
                    fika::util::make_sender("qm"),
                    fika::util::make_receiver("parse"))),
            std::move(parsers));
            // TODO(samuil): add more senders for the different parsers
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
  fika::log::log_info("Stopping parse service");

  return 0;
}
