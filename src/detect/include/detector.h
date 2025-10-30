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

#ifndef SRC_DETECT_INCLUDE_DETECTOR_H_
#define SRC_DETECT_INCLUDE_DETECTOR_H_

#include <magic.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "magic_handle.h"

namespace fika::detect {

struct detection_result {
  std::string mime_type;
  std::string source;
  double confidence{1.0};  // default 100%
};

class detector {
 public:
  explicit detector(std::vector<std::unique_ptr<file_detector>> detectors)
      : detectors_(std::move(detectors)) {}

  detection_result detect_file(const std::string &file);

 private:
  std::vector<std::unique_ptr<file_detector>> detectors_;
};

}  // namespace fika::detect

#endif  // SRC_DETECT_INCLUDE_DETECTOR_H_
