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

#ifndef SRC_SUPERVISOR_INCLUDE_SUPERVISOR_H_
#define SRC_SUPERVISOR_INCLUDE_SUPERVISOR_H_

#include <boost/interprocess/ipc/message_queue.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cmd.h"
#include "commands.h"
#include "ipc_queue_manager.h"
#include "queue_descriptor.h"
#include "supervisor_inter.h"
#include "worker.h"

namespace fika {

class supervisor : public supervisor_inter {
 public:
  using worker_factory_t = std::function<std::unique_ptr<worker>()>;

  explicit supervisor(const std::vector<worker_factory_t> &factories,
                      std::vector<fika::queue_descriptor> queues,
                      std::shared_ptr<ipc_queue_manager> queue_mgr);

  void run() override;

  void start_workers(const std::string &service_name = "") override;
  void stop_workers(const std::string &service_name = "") override;
  void handle_command(CommandType cmd_type,
                      const std::string &service_name = "") override;
  void reset_queues() override;
  void create_queues() override;
  std::vector<worker_entity> &get_workers() override { return workers_; }
  void monitor_once() override;
  void send_pong(const CommandResponse &msg) override;
  void register_commands() override;

 private:
  std::vector<queue_descriptor> queues_;
  std::vector<worker_entity> workers_;
  std::shared_ptr<ipc_queue_manager> queue_mgr_;
  std::map<CommandType, std::unique_ptr<cmd>> commands_;
  std::unique_ptr<boost::interprocess::message_queue> mq_receive;
  std::unique_ptr<boost::interprocess::message_queue> mq_send;
};

}  // namespace fika

#endif  // SRC_SUPERVISOR_INCLUDE_SUPERVISOR_H_
