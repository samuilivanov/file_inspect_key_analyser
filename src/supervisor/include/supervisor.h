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

#ifndef SUPERVISON_H
#define SUPERVISON_H

#include "cmd.h"
#include "commands.h"
#include "ipc_queue_manager.h"
#include "queue_descriptor.h"
#include "worker.h"
#include <memory>
#include <vector>

namespace fika {

class supervisor {
public:
  using worker_factory_t = std::function<std::unique_ptr<worker>()>;

  explicit supervisor(std::vector<worker_factory_t> factories,
                      std::vector<fika::queue_descriptor> queues,
                      std::shared_ptr<ipc_queue_manager> queue_mgr);

  virtual ~supervisor() = default;
  void run();

  virtual void start_workers(const std::string &service_name = "");
  virtual void stop_workers(const std::string &service_name = "");
  virtual void handle_command(CommandType cmd_type,
                              const std::string &service_name = "");
  virtual void reset_queues();
  virtual void create_queues();
  std::vector<queue_descriptor> queues_;

  virtual const std::vector<worker_entity> &get_workers() const {
    return workers_;
  }

  virtual void monitor_once();

private:
  virtual void register_commands();
  std::vector<worker_entity> workers_;
  std::shared_ptr<ipc_queue_manager> queue_mgr_;
  std::map<CommandType, std::unique_ptr<cmd>> commands_;
};

} // namespace fika

#endif