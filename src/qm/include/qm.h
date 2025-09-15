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

#ifndef SRC_QM_INCLUDE_QM_H_
#define SRC_QM_INCLUDE_QM_H_

#include <string>
#include <utility>
#include <vector>

#include "config.h"
#include "file_job.h"
#include "message_queues.h"
#include "msg.h"
#include "state_machine.h"

namespace fika {

class qm {
 public:
  std::pair<std::string, file_job_shm> add_job(file_job job) {
    job.status = Status::NEW;
    jobs_in_memory.push_back(job);
    log::log_info("Registed job {}", job.id);

    // Submit event drives the FSM
    return handle_event(jobs_in_memory.back(), Event::SUBMIT);
  }

  std::pair<std::string, file_job_shm> process_results(
      const file_job_shm &job) {
    auto it = std::find_if(jobs_in_memory.begin(), jobs_in_memory.end(),
                           [&](const file_job &j) { return j.id == job.id; });

    if (it == jobs_in_memory.end()) {
      log::log_info("adding new job and return");
      return add_job(job.to_file_job());
    }

    // FSM event mapping
    switch (job.status) {
      case Status::DETECTING:
        it->mime = job.mime;  // TODO(samuil): the in memory object should
                              // be updated not thsi bullshit
        return handle_event(*it, Event::DETECTION_OK);
      case Status::FAILED:
        return handle_event(*it, Event::DETECTION_FAIL);
      case Status::PARSING:
        return handle_event(*it, Event::PARSE_OK);
        break;
      case Status::DONE:
        log::log_info("Job {} is fully DONE", it->id);
        break;
      default:
        break;
    }
    return {"done", file_job_shm{}};
  }

  std::pair<std::string, file_job_shm> handle_event(file_job &job, Event ev) {
    file_job_shm j{};
    if (sm.apply(job, ev)) {
      j.from_file_job(job);
    }
    std::string qname;
    switch (job.type) {
      case JobType::DETECTOR:
        qname = "detect";
        break;
      case JobType::PARSER:
        qname = "parse";
        break;
      default:
        break;
    }
    log::log_info("sending to {} service", qname);
    return std::make_pair(qname, j);
  }

 private:
  state_machine sm;
  std::vector<file_job> jobs_in_memory;

  // void scan_new_files() {
  //   std::filesystem::path root = FIKA_SPOOL_NEW_DIR;
  //   log::log_debug("Scan for file in new using path: {}", root.string());

  //   for (const auto &f : std::filesystem::directory_iterator(root)) {
  //     if (std::filesystem::is_regular_file(f)) {
  //       file_job fjob;
  //       fjob.id = "somerandomid"; // TODO (samuil) a id generator should be
  //                                 // implemented
  //       fjob.path = f.path().string();
  //       add_job(fjob);
  //     }
  //   }
  // }
};
}  // namespace fika

#endif  // SRC_QM_INCLUDE_QM_H_
