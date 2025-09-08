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
#include "msg.h"
#include <array>
#include <filesystem>

namespace fika {
void qm::main_loop() {
  while (true) {
    handle_next_job();
  }
}
void qm::setup() {
  std::filesystem::path root = "/home/samuil/Projects/fika/tmp/var/spool/fika";
  std::array<std::string, 4> subdirs{"new", "processing", "done", "failed"};
  for (const auto &d : subdirs) {
    std::filesystem::path dir = root / d;
    if (std::filesystem::create_directories(dir)) {
      msg_logger::log_info(std::string("created dir: ") + dir.string());
    }
  }
  scan_new_files();
}

void qm::add_job(const file_job &job) { file_queue.push_back(job); }

void qm::handle_next_job() {
  if (file_queue.empty())
    return;
  auto job = file_queue.front();
  switch (job.status) {
  case file_job::STATUS::NEW:
    /* code */
    break;

  default:
    break;
  }

  if (job.status == file_job::STATUS::DONE)
    file_queue.pop_front();
}

void qm::scan_new_files() {
  std::filesystem::path root = "/home/samuil/Projects/fika/tmp/var/spool/fika/new";
  for (const auto &f : std::filesystem::directory_iterator(root)) {
    if (std::filesystem::is_regular_file(f)) {
      file_job fjob;
      fjob.id =
          "somerandomid"; // TODO (samuil) a id generator should be implemented
      fjob.path = f.path().string();
      fjob.status = file_job::STATUS::NEW;
      file_queue.push_back(fjob);
    }
  }
}

} // namespace fika
