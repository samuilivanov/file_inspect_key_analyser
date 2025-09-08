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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "file_job.h"
#include <functional>
#include <map>

namespace fika {

struct state_machine {
  std::map<std::pair<Status, Event>, std::function<Status(file_job &)>>
      transitions;

  state_machine();

  bool apply(file_job &job, Event ev);
};
} // namespace fika

#endif