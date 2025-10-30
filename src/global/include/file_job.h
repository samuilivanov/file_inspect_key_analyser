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

#ifndef SRC_GLOBAL_INCLUDE_FILE_JOB_H_
#define SRC_GLOBAL_INCLUDE_FILE_JOB_H_

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <variant>

#include "mime_type.h"

namespace fika {

constexpr size_t MAX_ID_SIZE = 64;
constexpr size_t MAX_PATH_SIZE = 256;

enum class MessageType : std::uint8_t { JOB, HEARTBEAT, COMMAND };

enum class Status : std::uint8_t { NEW, DETECTING, PARSING, DONE, FAILED };

enum class Event : std::uint8_t {
  SUBMIT,
  DETECTION_OK,
  DETECTION_FAIL,
  PARSE_OK,
  PARSE_FAIL
};

enum class JobType : std::uint8_t { DETECTOR, PARSER, NONE };

struct file_job {
  std::string job_id;
  std::filesystem::path path;
  int attempts = 0;
  Status status{};
  JobType type{};
  mime::Type mime{};
  bool stop = false;
};

// copy from file_job, returns false if truncation occurred

struct file_job_shm {
  std::array<char, MAX_ID_SIZE> job_id = {};
  std::array<char, MAX_PATH_SIZE> path = {};
  int attempts = 0;
  Status status{};
  JobType type{};
  mime::Type mime{};
};

[[nodiscard]] file_job to_file_job(file_job_shm job_shm);

[[nodiscard]] file_job_shm from_file_job(const file_job& fjob);

struct poison_pill {};
struct empty_msg {};

using ipc_message =
    std::variant<file_job_shm, poison_pill>;  // add more if needed
}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_FILE_JOB_H_
