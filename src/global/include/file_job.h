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

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

#include "mime_type.h"

namespace fika {

constexpr size_t MAX_ID_SIZE = 64;
constexpr size_t MAX_PATH_SIZE = 256;

enum class MessageType { JOB, HEARTBEAT, COMMAND };

enum class Status { NEW, DETECTING, PARSING, DONE, FAILED };
enum class Event { SUBMIT, DETECTION_OK, DETECTION_FAIL, PARSE_OK, PARSE_FAIL };
enum class JobType { DETECTOR, PARSER, NONE };

struct file_job {
  std::string id;
  std::filesystem::path path;
  int attempts = 0;
  Status status{};
  JobType type{};
  mime::Type mime{};
  bool stop = false;
};

struct file_job_shm {
  char id[MAX_ID_SIZE] = {};
  char path[MAX_PATH_SIZE] = {};
  int attempts = 0;
  Status status{};
  JobType type{};
  mime::Type mime{};
  bool stop = false;

  // copy from file_job, returns false if truncation occurred
  bool from_file_job(const file_job& fj) {
    bool truncated = false;

    if (fj.id.size() >= MAX_ID_SIZE) truncated = true;
    if (fj.path.string().size() >= MAX_PATH_SIZE) truncated = true;

    std::strncpy(id, fj.id.c_str(), MAX_ID_SIZE - 1);
    std::strncpy(path, fj.path.string().c_str(), MAX_PATH_SIZE - 1);

    attempts = fj.attempts;
    status = fj.status;
    type = fj.type;
    mime = fj.mime;
    stop = fj.stop;

    return !truncated;
  }

  file_job to_file_job() const {
    file_job fj;
    fj.id = id;
    fj.path = path;
    fj.attempts = attempts;
    fj.status = status;
    fj.type = type;
    fj.mime = mime;
    fj.stop = stop;
    return fj;
  }
};

struct ipc_message {
  MessageType type{};
  union {
    file_job_shm job;  // full job metadata
    int command_id;    // simple command (start/stop/ping)
  };

  // JOB constructor
  explicit ipc_message(const file_job_shm& j)
      : type(MessageType::JOB), job(j) {}

  // COMMAND constructor
  explicit ipc_message(int cmd) : type(MessageType::COMMAND), command_id(cmd) {}

  // Default constructor (optional)
  ipc_message() : type(MessageType::JOB), job{} {}
};

}  // namespace fika

#endif  // SRC_GLOBAL_INCLUDE_FILE_JOB_H_
