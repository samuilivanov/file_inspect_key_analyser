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

#ifndef MAGIC_HANDLE_H
#define MAGIC_HANDLE_H
#include <magic.h>
#include <string>
namespace fika::util {

class magic_handle {
private:
  magic_t handle;

public:
  explicit magic_handle(int flags = MAGIC_MIME_TYPE);
  ~magic_handle();
  std::string file(const std::string &file);
};

} // namespace fika::util

#endif