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

#ifndef SRC_PARSE_INCLUDE_PARSED_H_
#define SRC_PARSE_INCLUDE_PARSED_H_

#include "parser_registry.h"
#include "service.h"

namespace fika {
class parse_service : public Service {
 public:
  parse_service(std::unique_ptr<queue_receiver> receiver,
                fika::parser_registry parsers);
  void handle(ipc_message msg) override;
  fika::parser_registry parsers_;
};
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSED_H_
