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

#ifndef PARSER_SERVICE_H
#define PARSER_SERVICE_H

#include "ipc_client.h"
#include "parser_registry.h"
#include "parser_worker.h"
#include <atomic>
#include <boost/asio/thread_pool.hpp>
#include <thread>

namespace fika {

class parser_service {
public:
  parser_service(const std::string &inputQueue,
                 std::size_t workerCount = std::thread::hardware_concurrency());

  void run();
  void stop();

private:
  std::shared_ptr<ipc_client> client_;
  std::shared_ptr<parser_registry> registry_;
  std::shared_ptr<std::atomic<bool>> running_;
  std::unique_ptr<boost::asio::thread_pool> pool_;
  std::size_t workerCount_;
};
} // namespace fika

#endif