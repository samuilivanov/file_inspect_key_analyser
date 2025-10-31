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

#include "pid_file.h"

#include <unistd.h>

#include <boost/interprocess/sync/file_lock.hpp>
#include <filesystem>
#include <fstream>

#include "msg.h"

namespace fika::util {
bool pidfile_lock::create_and_lock() {
  // Ensure file exists
  if (!std::filesystem::exists(pid_path_)) {
    std::ofstream temp(pid_path_, std::ios::trunc);
    temp << getpid() << "\n";
    temp.flush();
  }
  lock_ = boost::interprocess::file_lock(pid_path_.c_str());

  // Try to lock the file
  if (!lock_.try_lock()) {
    log::log_error("PID file is already locked by another process");
    return false;
  }

  // Keep file open for lifetime
  pidfile_.open(pid_path_, std::ios::trunc);
  if (!pidfile_.is_open()) {
    log::log_error("Failed to open PID file for writing");
    lock_.unlock();  // release lock manually
    return false;
  }

  pidfile_ << getpid() << "\n";
  pidfile_.flush();

  return true;
}

bool pidfile_lock::is_locked(const std::string &pid_path) {
  if (!std::filesystem::exists(pid_path)) {
    return false;
  }

  try {
    boost::interprocess::file_lock temp_lock(pid_path.c_str());
    return !temp_lock.try_lock();
  } catch (const std::exception &e) {
    log::log_error("Failed to check PID file lock: %s", e.what());
    return false;
  }
}
pid_t pidfile_lock::read_pid(const std::string &pid_path) {
  std::ifstream file(pid_path);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open PID file: " + pid_path);
  }

  pid_t pid{};
  file >> pid;
  return pid;
}
}  // namespace fika::util