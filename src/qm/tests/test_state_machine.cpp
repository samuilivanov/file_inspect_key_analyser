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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "file_job.h"
#include "state_machine.h"

using namespace fika;

TEST_CASE("state_machine applies valid transitions") {
  state_machine sm;
  file_job job{};
  job.status = Status::NEW;
  job.type = JobType::DETECTOR;
  job.job_id = "job1";

  // NEW + SUBMIT => DETECTING
  bool applied = sm.apply(&job, Event::SUBMIT);
  REQUIRE(applied);
  REQUIRE(job.status == Status::DETECTING);
  REQUIRE(job.type == JobType::DETECTOR);

  // DETECTING + DETECTION_OK => PARSING
  applied = sm.apply(&job, Event::DETECTION_OK);
  REQUIRE(applied);
  REQUIRE(job.status == Status::PARSING);
  REQUIRE(job.type == JobType::PARSER);

  // PARSING + PARSE_OK => DONE
  applied = sm.apply(&job, Event::PARSE_OK);
  REQUIRE(applied);
  REQUIRE(job.status == Status::DONE);
}

TEST_CASE("state_machine handles failure transitions") {
  state_machine sm;

  file_job job1{};
  job1.status = Status::DETECTING;
  job1.type = JobType::DETECTOR;
  job1.job_id = "job2";
  bool applied = sm.apply(&job1, Event::DETECTION_FAIL);
  REQUIRE(applied);
  REQUIRE(job1.status == Status::FAILED);

  file_job job2{};
  job2.status = Status::PARSING;
  job2.type = JobType::PARSER;
  job2.job_id = "job3";
  applied = sm.apply(&job2, Event::PARSE_FAIL);
  REQUIRE(applied);
  REQUIRE(job2.status == Status::FAILED);
}

TEST_CASE("state_machine returns false for unknown transitions") {
  state_machine sm;

  file_job job{};
  job.status = Status::NEW;
  job.type = JobType::DETECTOR;
  job.job_id = "job4";
  // PARSE_OK from NEW is invalid
  bool applied = sm.apply(&job, Event::PARSE_OK);
  REQUIRE(!applied);
  REQUIRE(job.status == Status::NEW);
}
