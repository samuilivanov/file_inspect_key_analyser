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

#ifndef WORKER_H
#define WORKER_H

#include <boost/process.hpp>
#include <chrono>
#include <memory>
#include <string>

namespace fika {

class worker {
private:
  std::string path_;
  std::vector<std::string> args_;
  std::unique_ptr<boost::process::child> process_;
  std::chrono::steady_clock::time_point last_heartbeat_;

public:
  worker(std::string path, std::vector<std::string> args);
  void start();
  bool is_alive() const;
  void restart();
};
} // namespace fika

#endif