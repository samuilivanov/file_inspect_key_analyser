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

#include "parser_registry.h"
#include "parser.h"
#include "parsers.h"
namespace fika {

std::unique_ptr<parser>
parser_registry::create_parser(MimeType mimeType) const {
  switch (mimeType) {
  case MimeType::PDF:
    return std::make_unique<pdf_parser>();
  case MimeType::TEXT:
    return std::make_unique<text_parser>();
  case MimeType::IMAGE_PNG:
    return std::make_unique<img_parser>();
  default:
    return nullptr;
  }
}

parser *parser_registry::find_parser(MimeType mimeType) {
  auto it = parsers_.find(mimeType);
  if (it != parsers_.end())
    return it->second.get();

  if (auto psr = create_parser(mimeType)) {
    parser *ptr = psr.get();
    parsers_[mimeType] = std::move(psr);
    return ptr;
  }

  return nullptr;
}
} // namespace fika
