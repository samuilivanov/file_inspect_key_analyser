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

namespace fika::detect {

struct detection_result {
  std::string mime_type;
  std::string source;
};

class detector {
public:
  detector() : magic_cookie(MAGIC_MIME_TYPE) {}
  detection_result detect_file(const std::string &file);

private:
  util::magic_handle magic_cookie;
};

} // namespace fika::detect

#endif