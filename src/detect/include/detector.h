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

#ifndef DETECTOR_H
#define DETECTOR_H
#include "magic_handle.h"
#include <magic.h>
#include <string>
#include <vector>

namespace fika::detect {

struct detection_result {
  std::string mime_type;
  std::string source;
  double confidence{1.0}; // default 100%
};

class detector {
public:
  explicit detector(std::vector<std::unique_ptr<file_detector>> detectors)
      : detectors_(std::move(detectors)) {}

  detection_result detect_file(const std::string &file);

private:
  std::vector<std::unique_ptr<file_detector>> detectors_;
};

} // namespace fika::detect

#endif