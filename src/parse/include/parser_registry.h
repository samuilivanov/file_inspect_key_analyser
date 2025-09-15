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

#ifndef SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_
#define SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_

#include <memory>
#include <unordered_map>

#include "parser.h"

namespace fika {
class parser_registry {
 public:
  parser_registry() = default;

  parser_registry(const parser_registry &) = delete;
  parser_registry &operator=(const parser_registry &) = delete;

  parser *find_parser(MimeType mimeType);  // use int

 private:
  std::unordered_map<MimeType, std::unique_ptr<parser>> parsers_;

  std::unique_ptr<parser> create_parser(MimeType mimeType) const;
};
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_
