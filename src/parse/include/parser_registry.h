/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_
#define SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_

#include <memory>
#include <unordered_map>

#include "mime_type.h"
#include "parser.h"

namespace fika {
class parser_registry {
 public:
  parser_registry() = default;

  parser *find_parser(mime::Type mimeType);  // use int

 private:
  std::unordered_map<mime::Type, std::unique_ptr<parser>> parsers_;
};
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSER_REGISTRY_H_
