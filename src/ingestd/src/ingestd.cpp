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

#include "ingestd.h"
#include "file_job.h"
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <httplib.h>
#include <iostream>

namespace {
void generate_unique_id(char id[64]) {
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
  std::snprintf(id, 64, "job_%lld_%04x", static_cast<long long>(t), rand_hex);
}

} // namespace

namespace fika {

ingestd::ingestd(const std::string &spool_dir)
    : spool_dir_(spool_dir), mq_(boost::interprocess::open_or_create,
                                 "result_queue", 100, sizeof(file_job_shm)) {}

void ingestd::handle_file(const std::string &tmp_path,
                          const std::string &filename) {
  std::filesystem::path dst = std::filesystem::path(spool_dir_) / filename;
  try {
    std::filesystem::rename(tmp_path, dst);

  } catch (...) {
    std::cout << "error";
  }

  file_job_shm job{};
  generate_unique_id(job.id);
  std::strncpy(job.path, dst.c_str(), sizeof(job.path) - 1);

  mq_.send(&job, sizeof(job), 0);
}

void ingestd::run() {
  httplib::Server svr;

  svr.Post(
      "/upload", [this](const httplib::Request &req, httplib::Response &res) {
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
        std::filesystem::path tmp =
            "/home/samuil/Projects/fika/tmp/var/spool/fika/incomming";
        tmp /= filename;
        std::ofstream ofs(tmp, std::ios::binary);
        ofs.write(body.data(), body.size());
        ofs.close();

        handle_file(tmp.string(), filename);

        res.set_content("Job submitted\n", "text/plain");
      });

  std::cout << "ingestd HTTP server running on port 8080\n";
  svr.listen("0.0.0.0", 8080);
}

} // namespace fika
