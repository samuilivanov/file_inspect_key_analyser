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

#ifndef INGESTD_H
#define INGESTD_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

namespace fika {
class ingestd {
private:
  std::string spool_dir_;
  boost::interprocess::message_queue mq_;
  void handle_file(const std::string &tmp_path, const std::string &filename);

public:
  explicit ingestd(const std::string &spool_dir);
  void run();
};

} // namespace fika

#endif