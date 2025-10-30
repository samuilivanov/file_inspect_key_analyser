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

#include "qn_rslv.h"

namespace fika::util {
std::optional<std::pair<std::string, std::string>> parse_direct_queue(
    const std::string &queue_name) {
  const std::string prefix = "fika.direct.";
  const std::string sep = ".to.";

  if (queue_name.rfind(prefix, 0) != 0) {
    return std::nullopt;  // not matching
  }

  auto pos = queue_name.find(sep, prefix.size());
  if (pos == std::string::npos) {
    return std::nullopt;  // missing separator
  }

  std::string sender = queue_name.substr(prefix.size(), pos - prefix.size());
  std::string receiver = queue_name.substr(pos + sep.size());

  if (sender.empty() || receiver.empty()) {
    return std::nullopt;
  }

  return std::make_pair(sender, receiver);
}

std::optional<std::string> parse_inbox_queue(const std::string &queue_name) {
  const std::string prefix = "fika.inbox.";

  if (queue_name.rfind(prefix, 0) != 0) {
    return std::nullopt;  // not matching
  }

  std::string binary = queue_name.substr(prefix.size());
  if (binary.empty()) {
    return std::nullopt;  // no binary name
  }

  return binary;
}

}  // namespace fika::util