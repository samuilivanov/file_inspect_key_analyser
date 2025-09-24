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

#ifndef SRC_QM_INCLUDE_QMD_H_
#define SRC_QM_INCLUDE_QMD_H_

#include "qm.h"
#include "service.h"

namespace fika {
class qm_service : public Service {
 public:
  qm_service(std::unique_ptr<queue_receiver> receiver, qm queue_manager);
  void handle(ipc_message msg) override;

 private:
  qm queue_manager_;
};
}  // namespace fika

#endif  // SRC_QM_INCLUDE_QMD_H_
