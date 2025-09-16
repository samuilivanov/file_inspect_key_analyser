/*
 * This file is part of Fika.
 *
 * Copyright [2025] Samuil Ivanov
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

#ifndef SRC_QM_INCLUDE_STATE_MACHINE_H_
#define SRC_QM_INCLUDE_STATE_MACHINE_H_

#include <functional>
#include <map>
#include <utility>

#include "file_job.h"

namespace fika {

struct state_machine {
  std::map<std::pair<Status, Event>, std::function<Status(file_job &)>>
      transitions;

  state_machine();

  bool apply(file_job *job, Event ev);
};
}  // namespace fika

#endif  // SRC_QM_INCLUDE_STATE_MACHINE_H_
