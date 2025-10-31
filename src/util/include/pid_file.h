/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <boost/interprocess/sync/file_lock.hpp>
#include <filesystem>
#include <fstream>
#include <memory>

#include "msg.h"

namespace fika::util {
#include <unistd.h>  // getpid()

#include <boost/interprocess/sync/file_lock.hpp>
#include <fstream>
#include <string>

class pidfile_lock {
 private:
  boost::interprocess::file_lock lock_;
  std::ofstream pidfile_;
  std::string pid_path_;

 public:
  // Construct and associate with PID file
  explicit pidfile_lock(std::string pid_path)
      : pid_path_(std::move(pid_path)) {}

  // Create PID file, lock it, write PID, keep file open
  bool create_and_lock();

  static bool is_locked(const std::string &pid_path);

  static pid_t read_pid(const std::string &pid_path);
};

}  // namespace fika::util
