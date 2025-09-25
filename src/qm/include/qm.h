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
#include <unordered_map>
#include <utility>

#include "file_job.h"
#include "pickup.h"
#include "state_machine.h"

namespace fika {

class qm {
 public:
  std::pair<std::string, file_job_shm> add_job(file_job job);

  std::pair<std::string, file_job_shm> process_results(const file_job_shm &job);

  std::pair<std::string, file_job_shm> handle_event(file_job *job, Event event);

 private:
  state_machine sm;
  std::unordered_map<std::string, file_job> jobs_in_memory;
  pickup_service pickup;
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
