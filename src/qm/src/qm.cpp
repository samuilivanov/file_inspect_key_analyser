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

#include "qm.h"

#include "file_job.h"
#include "msg.h"

namespace fika {

std::pair<std::string, file_job_shm> qm::add_job(file_job job) {
  job.status = Status::NEW;
  std::string key = job.job_id;
  jobs_in_memory[key] = job;
  log::log_info("Registed job {}", key);

  // Submit event drives the FSM
  return handle_event(&job, Event::SUBMIT);
}

std::pair<std::string, file_job_shm> qm::process_results(
    const file_job_shm &job) {
  file_job extended_job = to_file_job(job);
  // FSM event mapping
  switch (extended_job.status) {
    case Status::NEW:
      log::log_info("adding new job {} and return", extended_job.job_id);
      return add_job(extended_job);
    case Status::DETECTING:
      return handle_event(&extended_job, Event::DETECTION_OK);
    case Status::FAILED:
      log::log_info("Job {} FAILED", extended_job.job_id);
      return {"fail", job};

    case Status::PARSING:
      return handle_event(&extended_job, Event::PARSE_OK);
      break;
    case Status::DONE:
      log::log_info("Job {} is fully DONE", std::string(extended_job.job_id));
      pickup.remove_file(extended_job.path);
      return {"null_sender", {}};
      break;
    default:
      break;
  }
  return {"null_sender", file_job_shm{}};
}

std::pair<std::string, file_job_shm> qm::handle_event(file_job *job,
                                                      Event event) {
  file_job_shm fjob{};
  if (sm.apply(job, event)) {
    fjob = from_file_job(*job);
  }
  std::string qname;
  switch (job->type) {
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
  return std::make_pair(qname, fjob);
}

// void qm::scan_new_files() {
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
}  // namespace fika