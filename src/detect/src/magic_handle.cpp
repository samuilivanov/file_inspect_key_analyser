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

#include "magic_handle.h"
#include <stdexcept>

namespace fika::util {
magic_handle::magic_handle(int flags) {
  handle = magic_open(flags);
  if (!handle)
    throw std::runtime_error("Failed to open libmagic handle");
  if (magic_load(handle, nullptr) != 0)
    throw std::runtime_error(magic_error(handle));
}

magic_handle::~magic_handle() {
  if (handle)
    magic_close(handle);
}
std::string magic_handle::file(const std::string &file) {
  return magic_file(handle, file.c_str());
}

} // namespace fika::util
