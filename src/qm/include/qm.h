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

#ifndef QM_H_INCLUDE
#define QM_H_INCLUDE

#include "file_job.h"
#include <deque>

namespace fika {

class qm {

public:
  void main_loop();
  // TODO (samuil) this inits the /var/spool/fika folders
  void setup();

private:
  std::deque<file_job> file_queue;
  // TODO (samuil)
  // Filesystem notifications(advanced, Linux - specific)
  // Use inotify to get a callback as soon as a file appears in /new.
  // Queue manager only enqueues jobs when notified.
  // Pros: Instant, efficient, scales well.
  // Cons: Slightly more complex; platform-specific.
  // Hybrid / async approach (future-proof)
  // Use Boost.Asio to handle job processing and polling or events
  // asynchronously. Allows multiple detectors/parsers to process jobs in
  // parallel.

  void add_job(const file_job &job); // called by pickup service or uploader
  void handle_next_job();
  void scan_new_files();
};
} // namespace fika

#endif