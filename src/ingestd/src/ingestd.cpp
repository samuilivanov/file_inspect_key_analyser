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

#include "ingestd.h"

#include <httplib.h>

#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"
#include "file_job.h"
#include "msg.h"

// TODO(samuil): move this to util
namespace {
std::string generate_unique_id() {
  // Timestamp in milliseconds
  auto now = std::chrono::system_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(
               now.time_since_epoch())
               .count();

  // Random 4-digit hex
  static thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0, 0xffff);
  int rand_hex = dist(rng);

  // Format: job_<timestamp>_<rand>
  std::string result = "job_";
  result.append(std::to_string(t)).append(std::to_string(rand_hex));
  return result;
}

}  // namespace

namespace fika {

ingestd::ingestd(std::string_view spool_dir)
    : spool_dir_(spool_dir),
      mq_(boost::interprocess::open_only, "result_queue") {}

void ingestd::handle_file(const std::string &tmp_path,
                          const std::string &filename) {
  std::filesystem::path dst = std::filesystem::path(spool_dir_) / filename;
  std::filesystem::rename(tmp_path, dst);

  file_job_shm job{};

  auto uuid = generate_unique_id();
  auto len = uuid.copy(job.job_id.data(), job.job_id.size() - 1);
  job.job_id.at(len) = '\0';

  len = dst.string().copy(job.path.data(), job.path.size() - 1);
  job.path.at(len) = '\0';
  log::log_info("File job created with id: {}", std::string(job.job_id.data()));
  fika::ipc_message msg{job};
  mq_.send(&msg, sizeof(msg), 0);
  log::log_info("File job {} send for processing",
                std::string(job.job_id.data()));
}

void ingestd::run() {
  // TODO(samuil): httplib should be replaced with boost beast
  httplib::Server svr;

  svr.Post(
      "/upload", [this](const httplib::Request &req, httplib::Response &res) {
        log::log_debug("Accepting file!");
        // Read raw body as file content
        const auto &body = req.body;
        if (body.empty()) {
          res.status = 400;
          res.set_content("No data in request\n", "text/plain");
          return;
        }

        // Simple filename from header or fallback
        std::string filename =
            "upload_" + std::to_string(std::chrono::system_clock::to_time_t(
                            std::chrono::system_clock::now()));
        std::filesystem::path tmp = FIKA_SPOOL_INCOMMING_DIR;
        tmp /= filename;
        std::ofstream ofs(tmp, std::ios::binary);
        ofs.write(body.data(), body.size());
        ofs.close();
        log::log_info("file written to incomming: {}", filename);

        handle_file(tmp.string(), filename);

        res.set_content("Job submitted\n", "text/plain");
      });

  log::log_debug("ingestd HTTP server running on port 8080");
  svr.listen("0.0.0.0", 8080);
}

}  // namespace fika
