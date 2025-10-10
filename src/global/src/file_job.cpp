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

#include "file_job.h"

namespace fika {
[[nodiscard]] file_job to_file_job(file_job_shm job_shm) {
  file_job fjob;
  fjob.job_id = job_shm.job_id.data();
  fjob.path = job_shm.path.data();
  fjob.attempts = job_shm.attempts;
  fjob.status = job_shm.status;
  fjob.type = job_shm.type;
  fjob.mime = job_shm.mime;
  return fjob;
}

[[nodiscard]] file_job_shm from_file_job(const file_job& fjob) {
  file_job_shm job_shm;

  std::strncpy(job_shm.job_id.data(), fjob.job_id.c_str(), MAX_ID_SIZE - 1);
  std::strncpy(job_shm.path.data(), fjob.path.string().c_str(),
               MAX_PATH_SIZE - 1);

  job_shm.attempts = fjob.attempts;
  job_shm.status = fjob.status;
  job_shm.type = fjob.type;
  job_shm.mime = fjob.mime;

  return job_shm;
}
}  // namespace fika