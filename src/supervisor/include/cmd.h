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

#ifndef COMMAND_H_INCLUDE
#define COMMAND_H_INCLUDE
#include <string>

namespace fika {
class supervisor;
class cmd {
public:
  virtual ~cmd() = default;
  virtual void execute(supervisor &sup,
                       const std::string &service_name = "") = 0;
};

} // namespace fika

// You can add ReloadCommand, StatusCommand, etc.

#endif