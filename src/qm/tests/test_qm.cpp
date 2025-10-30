/*
 * This file is part of file_ingest_key_analyser (FIKA).
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cassert>
#include <queue>

#include "file_job.h"
#include "message_queues.h"
#include "qipc.h"
#include "qm.h"

using namespace fika;

struct mock_ipc {
  std::queue<file_job_shm> received_results;
  std::vector<file_job_shm> sent_jobs;

  void send_job(const file_job_shm &job) { sent_jobs.push_back(job); }

  file_job_shm receive_result() {
    REQUIRE(!received_results.empty());
    auto job = received_results.front();
    received_results.pop();
    return job;
  }
};

TEST_CASE("qm sends job to correct worker") {
  mock_ipc ipc;
  qm q(ipc);

  file_job job{};
  job.id = "job1";
  job.status = Status::NEW;
  job.type = JobType::DETECTOR;
  q.add_job(job);

  // The real state_machine changes job type to DETECTOR for SUBMIT
  REQUIRE(!ipc.sent_jobs.empty());
  CHECK_EQ(std::strcmp(ipc.sent_jobs[0].id, "job1"), 0);
}

TEST_CASE("add_job sends job to detector via IPC") {
  mock_ipc ipc;
  qm q(ipc);

  file_job job{};
  job.id = "job1";
  job.status = Status::NEW;
  job.type = JobType::DETECTOR;
  q.add_job(job);

  REQUIRE(ipc.sent_jobs.size() == 1);
  CHECK_EQ(std::strcmp(ipc.sent_jobs[0].id, "job1"), 0);
  REQUIRE(ipc.sent_jobs[0].type == JobType::DETECTOR);
}

TEST_CASE("process_result_once updates existing job") {
  mock_ipc ipc;
  qm q(ipc);

  file_job job{};
  job.id = "job1";
  job.status = Status::NEW;
  job.type = JobType::DETECTOR;
  q.add_job(job);

  file_job_shm result;
  std::memcpy(result.id, "job1", 5);
  result.status = Status::DETECTING;
  result.mime = MimeType::TEXT;
  ipc.received_results.push(result);

  q.process_results();

  REQUIRE(ipc.sent_jobs.size() ==
          2);  // add_job + DETECTION_OK triggers send_to_worker
  CHECK_EQ(std::strcmp(ipc.sent_jobs[0].id, "job1"), 0);
  REQUIRE(ipc.sent_jobs[1].mime == MimeType::TEXT);
}

// TEST_CASE("process_result_once adds new job if missing") {
//   mock_ipc ipc;
//   qm q(ipc);

//   file_job_shm result;
//   std::memcpy(result.id, "job_new", 7);
//   result.status = Status::DETECTING;
//   result.mime = MimeType::IMAGE_PNG;
//   ipc.received_results.push(result);

//   q.process_results();

//   REQUIRE(ipc.sent_jobs.size() == 1);
//   CHECK_EQ(std::strcmp(ipc.sent_jobs[0].id, "job_new"), 0);
//   REQUIRE(ipc.sent_jobs[0].mime == MimeType::IMAGE_PNG);
// }

TEST_CASE("process_result_once handles DONE result without sending") {
  mock_ipc ipc;
  qm q(ipc);

  file_job job{};
  job.id = "job_done";
  job.status = Status::PARSING;
  job.type = JobType::PARSER;
  q.add_job(job);

  file_job_shm result;
  std::memcpy(result.id, "job_done", 9);

  result.status = Status::DONE;
  ipc.received_results.push(result);

  q.process_results();

  // No new job sent to workers for DONE status
  REQUIRE(ipc.sent_jobs.size() == 1);  // only from add_job
}