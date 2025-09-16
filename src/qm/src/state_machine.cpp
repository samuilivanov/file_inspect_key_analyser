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

#include "state_machine.h"

namespace fika {

state_machine::state_machine() {
  transitions[{Status::NEW, Event::SUBMIT}] = [](file_job &job) {
    job.type = JobType::DETECTOR;
    return Status::DETECTING;
  };
  transitions[{Status::DETECTING, Event::DETECTION_OK}] = [](file_job &job) {
    job.type = JobType::PARSER;
    return Status::PARSING;
  };

  transitions[{Status::DETECTING, Event::DETECTION_FAIL}] = [](file_job &) {
    return Status::FAILED;
  };

  transitions[{Status::PARSING, Event::PARSE_OK}] = [](file_job &) {
    return Status::DONE;
  };

  transitions[{Status::PARSING, Event::PARSE_FAIL}] = [](file_job &) {
    return Status::FAILED;
  };
}

bool state_machine::apply(file_job *job, Event ev) {
  auto key = std::make_pair(job->status, ev);
  auto it = transitions.find(key);
  if (it == transitions.end()) {
    return false;
  }
  job->status = it->second(*job);
  return true;
}

}  // namespace fika
