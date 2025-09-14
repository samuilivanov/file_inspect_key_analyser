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
#include "file_job.h"
#include "message_queues.h"
#include "qipc.h"
#include <cassert>
#include <doctest/doctest.h>
#include <queue>
#include <vector>

using namespace fika;

struct mock_sender {
  std::vector<file_job_shm> sent;

  void send(const file_job_shm &job) { sent.push_back(job); }
};

struct mock_receiver {
  std::queue<file_job_shm> to_receive;

  file_job_shm receive() {
    assert(!to_receive.empty());
    auto job = to_receive.front();
    to_receive.pop();
    return job;
  }
};

TEST_CASE("qipc sends DETECTOR jobs correctly") {
  mock_sender detector;
  mock_sender parser;
  mock_receiver result;

  qipc<mock_sender, mock_sender, mock_receiver> ipc{detector, parser, result};

  file_job_shm job{};
  job.type = JobType::DETECTOR;
  ipc.send_job(job);

  REQUIRE(ipc.mq_detector.sent.size() == 1);
  REQUIRE(ipc.mq_detector.sent[0].type == JobType::DETECTOR);
  REQUIRE(ipc.mq_parser.sent.empty());
}

TEST_CASE("qipc sends PARSER jobs correctly") {
  mock_sender detector;
  mock_sender parser;
  mock_receiver result;

  qipc<mock_sender, mock_sender, mock_receiver> ipc{detector, parser, result};

  file_job_shm job{};
  job.type = JobType::PARSER;
  ipc.send_job(job);

  REQUIRE(ipc.mq_parser.sent.size() == 1);
  REQUIRE(ipc.mq_parser.sent[0].type == JobType::PARSER);
  REQUIRE(ipc.mq_detector.sent.empty());
}

TEST_CASE("qipc ignores NONE job type") {
  mock_sender detector;
  mock_sender parser;
  mock_receiver result;

  qipc<mock_sender, mock_sender, mock_receiver> ipc{detector, parser, result};

  file_job_shm job{};
  job.type = JobType::NONE;
  ipc.send_job(job);

  REQUIRE(ipc.mq_detector.sent.empty());
  REQUIRE(ipc.mq_parser.sent.empty());
}

TEST_CASE("qipc receives result correctly") {
  mock_sender detector;
  mock_sender parser;
  mock_receiver result;
  file_job_shm job{};
  job.type = JobType::DETECTOR;

  result.to_receive.push(job);

  qipc<mock_sender, mock_sender, mock_receiver> ipc{detector, parser, result};

  auto received = ipc.receive_result();
  REQUIRE(received.type == JobType::DETECTOR);
  REQUIRE(ipc.mq_result.to_receive.empty());
}