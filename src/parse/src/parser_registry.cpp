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

#include "parser_registry.h"

#include "mime_type.h"
#include "parser.h"
#include "parsers.h"

namespace {

std::unique_ptr<fika::parser> create_parser(fika::mime::Type mimeType) {
  switch (mimeType) {
    case fika::mime::Type::Pdf:
      return nullptr;
    case fika::mime::Type::TextPlain:
      return nullptr;
    case fika::mime::Type::ImageJpeg:
      return nullptr;
    default:
      return nullptr;
  }
}
}  // namespace

namespace fika {

parser *parser_registry::find_parser(mime::Type mimeType) {
  auto iter = parsers_.find(mimeType);
  if (iter != parsers_.end()) {
    return iter->second.get();
  }

  if (auto psr = create_parser(mimeType)) {
    parser *ptr = psr.get();
    parsers_[mimeType] = std::move(psr);
    return ptr;
  }

  return nullptr;
}
}  // namespace fika
