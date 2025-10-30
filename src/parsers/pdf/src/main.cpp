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

#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-embedded-file.h>
#include <poppler/cpp/poppler-page.h>

#include <iostream>

#include "file_job.h"
#include "pdf_parser.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: pdf <file.pdf>\n";
    return 1;
  }

  std::string filename = argv[1];
  fika::pdf_parser parser;

  fika::file_job job;
  job.path = filename;
  fika::file_job_shm j_shm;
  j_shm = from_file_job(job);
  parser.parse(j_shm);
  std::cout << parser.get_text() << '\n';
  return 0;
}
