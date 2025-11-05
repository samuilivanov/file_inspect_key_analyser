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

#include <filesystem>

#include "cli_parser.h"
#include "config.h"
#include "ingestd.h"
#include "msg.h"
#include "pid_file.h"

int main(int argc, char *argv[]) {
  fika::log::msg_logger_init();
  fika::log::log_info("Starting ingest");
  fika::cli_parser cli(argc, argv);
  cli.initialize(fika::default_option::pidfile);
  auto vm = cli.parse();
  if (!vm.has_value()) {
    return -1;
  }

  auto pid_path = cli.get_option<std::string>("pidfile");
  fika::util::pidfile_lock pidfile(pid_path);
  if (!pidfile.create_and_lock()) {
    return -1;
  }

  fika::ingestd ingest(FIKA_SPOOL_NEW_DIR);
  ingest.run();
  return 0;
}
