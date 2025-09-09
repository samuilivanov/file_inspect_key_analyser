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

#ifndef FILE_JOB_H
#define FILE_JOB_H

#include <cstring>
#include <filesystem>
#include <string>

namespace fika {

// Max sizes
constexpr std::size_t MAX_ID_SIZE = 64;
constexpr std::size_t MAX_PATH_SIZE = 256;

enum class Status { NEW, DETECTING, PARSING, DONE, FAILED };
enum class Event { SUBMIT, DETECTION_OK, DETECTION_FAIL, PARSE_OK, PARSE_FAIL };
enum class JobType { DETECTOR, PARSER, NONE };

struct file_job {
  std::string id;
  std::filesystem::path path;
  int attempts = 0;
  Status status;
  JobType type;
};

struct file_job_shm {
  char id[MAX_ID_SIZE] = {};
  char path[MAX_PATH_SIZE] = {};
  int attempts = 0;
  Status status;
  JobType type;

  void from_file_job(const file_job &fj) {
    std::strncpy(id, fj.id.c_str(), MAX_ID_SIZE - 1);
    std::strncpy(path, fj.path.string().c_str(), MAX_PATH_SIZE - 1);
    attempts = fj.attempts;
    status = fj.status;
    type = fj.type;
  }

  file_job to_file_job() const {
    file_job fj;
    fj.id = id;
    fj.path = path;
    fj.attempts = attempts;
    fj.status = status;
    fj.type = type;
    return fj;
  }
};

} // namespace fika

#endif