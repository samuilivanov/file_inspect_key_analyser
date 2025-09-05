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

#ifndef MSG_H_INCLUDED
#define MSG_H_INCLUDED

#include <boost/log/attributes/constant.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <fmt/core.h>
#include <memory>
#include <source_location>
#include <string>
namespace msg_logger {

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

void msg_logger_init(const std::string &log_file = "app.log");

// Convenience macros to capture file/line automatically

inline std::string_view filename_only(std::string_view path) {
  auto pos = path.find_last_of("/\\");
    if (pos == std::string_view::npos) return path;
  return path.substr(pos + 1);
}
inline void
log_debug(const std::string &msg,
          const std::source_location &loc = std::source_location::current()) {
  BOOST_LOG_TRIVIAL(debug) << fmt::format(
      "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
}
inline void
log_info(const std::string &msg,
         const std::source_location &loc = std::source_location::current()) {
  BOOST_LOG_TRIVIAL(info) << fmt::format(
      "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
}
inline void
log_warn(const std::string &msg,
         const std::source_location &loc = std::source_location::current()) {
  BOOST_LOG_TRIVIAL(warning) << fmt::format(
      "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
}
inline void
log_error(const std::string &msg,
          const std::source_location &loc = std::source_location::current()) {
  BOOST_LOG_TRIVIAL(error) << fmt::format(
      "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
}
inline void log_critical(
    const std::string &msg,
    const std::source_location &loc = std::source_location::current()) {
  BOOST_LOG_TRIVIAL(fatal) << fmt::format(
      "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
}
} // namespace msg_logger
#endif