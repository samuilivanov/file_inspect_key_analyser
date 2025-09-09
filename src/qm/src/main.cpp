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

#include "msg.h"
#include "qm.h"
#include <file_job.h>
#include <thread>

int main(int argc, char const *argv[]) {
  msg_logger::msg_logger_init("qm.log");

  fika::qm qm;
  qm.setup();

  // Start results polling
  std::thread result_thread([&qm] {
    while (true) {
      qm.process_results();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });

  result_thread.join();
  return 0;
}
