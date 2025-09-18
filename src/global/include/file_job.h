/*
 * This file is part of Fika.
 *
 * Copyright [2025] Samuil Ivanov
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

struct file_job_shm {
  std::array<char, MAX_ID_SIZE> job_id = {};
  std::array<char, MAX_PATH_SIZE> path = {};
  int attempts = 0;
  Status status{};
  JobType type{};
  mime::Type mime{};
  bool stop = false;

  // copy from file_job, returns false if truncation occurred
  [[nodiscard]] bool from_file_job(const file_job& fjob) {
    bool truncated = false;

    if (fjob.job_id.size() >= MAX_ID_SIZE) {
      truncated = true;
    }
    if (fjob.path.string().size() >= MAX_PATH_SIZE) {
      truncated = true;
    }

    std::strncpy(job_id.data(), fjob.job_id.c_str(), MAX_ID_SIZE - 1);
    std::strncpy(path.data(), fjob.path.string().c_str(), MAX_PATH_SIZE - 1);

    attempts = fjob.attempts;
    status = fjob.status;
    type = fjob.type;
    mime = fjob.mime;
    stop = fjob.stop;

    return !truncated;
  }

  [[nodiscard]] file_job to_file_job() const {
    file_job fjob;
    fjob.job_id = job_id.data();
    fjob.path = path.data();
    fjob.attempts = attempts;
    fjob.status = status;
    fjob.type = type;
    fjob.mime = mime;
    fjob.stop = stop;
    return fjob;
  }
};
using ipc_message = std::variant<file_job_shm, int>;  // add more if needed
}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_FILE_JOB_H_
