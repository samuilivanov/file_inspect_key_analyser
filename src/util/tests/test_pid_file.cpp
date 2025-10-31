/*
 * This file is part of file_ingest_key_analyser (FIKA).
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include "doctest/doctest.h"
#include "pid_file.h"

using namespace fika::util;

static std::string make_temp_pidfile() {
  std::string tmp = "/tmp/test_pidfile_" + std::to_string(::getpid()) + "_" +
                    std::to_string(::rand());
  return tmp;
}

TEST_CASE("pidfile_lock::create_and_lock creates file and writes pid") {
  std::string path = make_temp_pidfile();

  pidfile_lock lock1(path);
  REQUIRE(lock1.create_and_lock() == true);

  pid_t pid = fork();
  REQUIRE(pid >= 0);

  if (pid == 0) {
    // child process
    bool locked = pidfile_lock::is_locked(path);
    std::exit(locked ? 0 : 1);
  } else {
    int status{};
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);  // means locked
  }

  std::filesystem::remove(path);
}

TEST_CASE("pidfile_lock::is_locked returns false if no file") {
  std::string path = make_temp_pidfile();
  CHECK(pidfile_lock::is_locked(path) == false);
}

TEST_CASE("pidfile_lock::read_pid reads correct PID") {
  std::string path = make_temp_pidfile();
  pid_t pid = ::getpid();

  {
    std::ofstream f(path);
    f << pid << "\n";
  }

  CHECK(pidfile_lock::read_pid(path) == pid);

  std::filesystem::remove(path);
}

TEST_CASE("pidfile_lock::is_locked returns true when locked elsewhere") {
  std::string path = make_temp_pidfile();

  // Create and lock
  pidfile_lock lock1(path);
  REQUIRE(lock1.create_and_lock() == true);

  // Another call should see it as locked
  pid_t pid = fork();
  REQUIRE(pid >= 0);

  if (pid == 0) {
    // child process
    bool locked = pidfile_lock::is_locked(path);
    std::exit(locked ? 0 : 1);
  } else {
    int status{};
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);  // means locked
  }

  std::filesystem::remove(path);
}

TEST_CASE("pidfile_lock::create_and_lock fails if already locked") {
  std::string path = make_temp_pidfile();

  pidfile_lock lock1(path);
  REQUIRE(lock1.create_and_lock() == true);

  pid_t pid = fork();
  REQUIRE(pid >= 0);

  if (pid == 0) {
    // child process
    pidfile_lock lock2(path);
    bool locked = lock2.create_and_lock();
    std::exit(locked ? 0 : 1);
  } else {
    int status{};
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 1);  // means locked
  }

  std::filesystem::remove(path);
}
