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

#include "ipc_client.h"
namespace fika {
void MsgQueueReceiver::receive(ipc_message &job) {
  size_t recv_size{};
  unsigned int priority{};
  mq_recv_.receive(&job, sizeof(ipc_message), recv_size, priority);
}

void MsgQueueSender::send(const ipc_message &result) {
  mq_send_.send(&result, sizeof(ipc_message), 0);
}
}  // namespace fika