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

#ifndef PERSER_WORKER_H
#define PERSER_WORKER_H

#include "ipc_client.h"
#include "parser_registry.h"
#include <atomic>

namespace fika {
class parser_worker {
public:
  parser_worker(std::shared_ptr<ipc_client> client,
                std::shared_ptr<std::atomic<bool>> running,
                std::shared_ptr<parser_registry> registry);

  void operator()();

private:
  std::shared_ptr<ipc_client> client_;
  std::shared_ptr<std::atomic<bool>> running_;
  std::shared_ptr<parser_registry> registry_;

  void process_job(file_job_shm &job);
};

} // namespace fika

#endif