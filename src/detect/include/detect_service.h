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

#ifndef SRC_DETECT_INCLUDE_DETECTD_
#define SRC_DETECT_INCLUDE_DETECTD_

#include "detector.h"
#include "service.h"

namespace fika {

class detect_service : public Service {
 public:
  detect_service(std::unique_ptr<queue_receiver> receiver,
                 detect::detector detect);
  void handle(ipc_message msg) override;

 private:
  detect::detector detect_;
};
}  // namespace fika

#endif  // SRC_DETECT_INCLUDE_DETECTD_
