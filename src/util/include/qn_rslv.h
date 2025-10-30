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

#ifndef SRC_UTIL_INCLUDE_QN_RSLV_H_
#define SRC_UTIL_INCLUDE_QN_RSLV_H_

#include <optional>
#include <string>

namespace fika::util {

struct sender_id {
  std::string name;
  explicit sender_id(std::string sender) : name(std::move(sender)) {}
};

struct receiver_id {
  std::string name;
  explicit receiver_id(std::string receiver) : name(std::move(receiver)) {}
};

inline sender_id make_sender(std::string sender) {
  return sender_id(std::move(sender));
}
inline receiver_id make_receiver(std::string receiver) {
  return receiver_id(std::move(receiver));
}

inline std::string get_direct_queue(const sender_id &from,
                                    const receiver_id &to) {
  return "fika.direct." + from.name + ".to." + to.name;
}

inline std::string get_inbox_queue(const receiver_id &to) {
  return "fika.inbox." + to.name;
}

// remove the accidental plain-string API to force new code to use strong types
inline std::string get_direct_queue(const std::string &from,
                                    const std::string &to) = delete;
inline std::string get_inbox_queue(const std::string &to) = delete;

std::optional<std::pair<std::string, std::string>> parse_direct_queue(
    const std::string &queue_name);

std::optional<std::string> parse_inbox_queue(const std::string &queue_name);
}  // namespace fika::util

#endif  // SRC_UTIL_INCLUDE_QN_RSLV_H_
