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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <string>

#include "../include/file_job.h"

using namespace fika;

TEST_CASE("file_job_shm copies all fields and roundtrips") {
  file_job fj;
  fj.id = "job_123";
  fj.path = "/tmp/file.txt";
  fj.attempts = 2;
  fj.status = Status::DETECTING;
  fj.type = JobType::DETECTOR;
  fj.mime = mime::Type::Pdf;
  fj.stop = true;

  file_job_shm shm;
  bool ok = shm.from_file_job(fj);

  CHECK(ok == true);
  CHECK(std::strcmp(shm.id, "job_123") == 0);
  CHECK(std::strcmp(shm.path, "/tmp/file.txt") == 0);
  CHECK(shm.attempts == 2);
  CHECK(shm.status == Status::DETECTING);
  CHECK(shm.type == JobType::DETECTOR);
  CHECK(shm.mime == mime::Type::Pdf);
  CHECK(shm.stop == 1);

  file_job fj2 = shm.to_file_job();
  CHECK(fj2.id == fj.id);
  CHECK(fj2.path == fj.path);
  CHECK(fj2.attempts == fj.attempts);
  CHECK(fj2.status == fj.status);
  CHECK(fj2.type == fj.type);
  CHECK(fj2.mime == fj.mime);
  CHECK(fj2.stop == fj.stop);
}

TEST_CASE("Truncation detection for long id and path") {
  std::string long_id(MAX_ID_SIZE + 10, 'X');
  std::string long_path(MAX_PATH_SIZE + 20, 'Y');

  file_job fj;
  fj.id = long_id;
  fj.path = long_path;
  fj.status = Status::PARSING;
  fj.type = JobType::PARSER;
  fj.mime = mime::Type::MsOfficeWord;
  fj.stop = false;

  file_job_shm shm;
  bool ok = shm.from_file_job(fj);

  CHECK(ok == false);  // truncation occurred
  CHECK(strlen(shm.id) == MAX_ID_SIZE - 1);
  CHECK(strlen(shm.path) == MAX_PATH_SIZE - 1);

  file_job fj2 = shm.to_file_job();
  CHECK(fj2.id.size() == MAX_ID_SIZE - 1);
  CHECK(fj2.path.string().size() == MAX_PATH_SIZE - 1);
}

TEST_CASE("Default constructed file_job_shm is zeroed") {
  file_job_shm shm;
  CHECK(std::strlen(shm.id) == 0);
  CHECK(std::strlen(shm.path) == 0);
  CHECK(shm.attempts == 0);
  CHECK(shm.stop == 0);
}
