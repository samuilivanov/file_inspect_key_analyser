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

#include "qm.h"
#include "config.h"
#include "msg.h"
#include <algorithm>
#include <array>
#include <filesystem>

namespace fika {
void qm::setup() {
  // TODO (samuil) redesign this setup function
  std::filesystem::path root = FIKA_SPOOL_DIR.data();
  log::log_debug("Setup starting using path: {}", root.string());
  std::array<std::string, 5> subdirs{"incomming", "new", "processing", "done",
                                     "failed"};
  for (const auto &d : subdirs) {
    std::filesystem::path dir = root / d;
    if (std::filesystem::create_directories(dir)) {
      log::log_info(std::string("created dir: ") + dir.string());
    }
  }
  // scan_new_files();
}

void qm::add_job(file_job job) {
  job.status = Status::NEW;
  jobs_in_memory.push_back(job);
  log::log_info("Registed job {}", job.id);

  // Submit event drives the FSM
  handle_event(jobs_in_memory.back(), Event::SUBMIT);
}

void qm::process_results() {
  file_job_shm result{};
  while (true) {
    result = ipc.receive_result();
    // Add job if missing
    auto it =
        std::find_if(jobs_in_memory.begin(), jobs_in_memory.end(),
                     [&](const file_job &j) { return j.id == result.id; });

    if (it == jobs_in_memory.end()) {
      add_job(result.to_file_job());
      it = std::prev(jobs_in_memory.end());
    }

    // FSM event mapping
    switch (result.status) {
    case Status::DETECTING:
      it->mime = result.mime; // TODO (samuil) the in memory object should be
                              // updated not thsi bullshit
      handle_event(*it, Event::DETECTION_OK);
      break;
    case Status::FAILED:
      handle_event(*it, Event::DETECTION_FAIL);
      break;
    case Status::PARSING:
      handle_event(*it, Event::PARSE_OK);
      break;
    case Status::DONE:
      log::log_info("Job {} is fully DONE", it->id);
      break;
    default:
      break;
    }
  }
}

void qm::handle_event(file_job &job, Event ev) {
  if (sm.apply(job, ev)) {
    send_to_worker(job);
  }
}

void qm::send_to_worker(const file_job &job) {
  file_job_shm jobs{};
  jobs.from_file_job(job);
  // TODO (samuil) this can be a map with the different services that gets the
  // string and in the log just put that instead of having if else just for the
  // log to be different
  if (job.type == JobType::DETECTOR) {
    ipc.send_job(jobs);
    log::log_info("Sent job {} to detectd", job.id);
  } else if (job.type == JobType::PARSER) {
    ipc.send_job(jobs);
    log::log_info("Sent job {} to parsed", job.id);
  }
}

void qm::scan_new_files() {
  std::filesystem::path root = FIKA_SPOOL_NEW_DIR;
  log::log_debug("Scan for file in new using path: {}", root.string());

  for (const auto &f : std::filesystem::directory_iterator(root)) {
    if (std::filesystem::is_regular_file(f)) {
      file_job fjob;
      fjob.id =
          "somerandomid"; // TODO (samuil) a id generator should be implemented
      fjob.path = f.path().string();
      add_job(fjob);
    }
  }
}

} // namespace fika
