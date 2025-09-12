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

#ifndef QUEUE_DESCRIPTOR
#define QUEUE_DESCRIPTOR
#include <ctype.h>
#include <string>

namespace fika {
struct queue_descriptor {
  std::string name;
  std::size_t max_messages;
  std::size_t message_size;
};

} // namespace fika

#endif