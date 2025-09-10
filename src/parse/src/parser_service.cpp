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

#include "parser_service.h"
#include <boost/asio/post.hpp>

namespace fika {

parser_service::parser_service(const std::string &inputQueue,
                               std::size_t workerCount)
    : client_(std::make_shared<ipc_client>(inputQueue)),
      registry_(std::make_shared<parser_registry>()),
      running_(std::make_shared<std::atomic<bool>>(false)), pool_(nullptr),
      workerCount_(workerCount) {}

void parser_service::run() {
  running_->store(true);

  // Create thread pool with specified worker count
  pool_ = std::make_unique<boost::asio::thread_pool>(workerCount_);

  // Post workers
  for (std::size_t i = 0; i < workerCount_; ++i) {
    boost::asio::post(
        *pool_, [client = client_, running = running_, registry = registry_] {
          parser_worker worker(client, running, registry);
          worker();
        });
  }

  // Wait for all threads
  pool_->join();
}

void parser_service::stop() {
  running_->store(false);
  if (pool_) {
    pool_->stop();
  }
}

} // namespace fika
