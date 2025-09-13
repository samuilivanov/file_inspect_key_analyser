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

#ifndef FILE_DETECTOR_H
#define FILE_DETECTOR_H
#include <string>

namespace fika {
struct file_detector {
  virtual ~file_detector() = default;
  virtual std::string detect(const std::string &filepath) = 0;
  virtual std::string name() const = 0; // source identifier, e.g., "libmagic"
};

} // namespace fika

#endif