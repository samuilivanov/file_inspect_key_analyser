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
#include <detector.h>
#include <iostream>

int main(int argc, char const *argv[]) {
  if (argc < 2) {

    std::cerr << "Usage: detect <file1> [file2 ...]\n";
    return 1;
  }
  msg_logger::msg_logger_init("detect.log");
  fika::detect::detector d;
  for (int i = 1; i < argc; ++i) {
    auto result = d.detect_file(argv[i]);
    msg_logger::log_info("Detect file: {}: {}", argv[i], result.mime_type);
    std::cout << argv[i] << ": " << result.mime_type << " (" << result.source
              << ")\n";
  }
  return 0;
}
