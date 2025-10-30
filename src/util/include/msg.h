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

#ifndef SRC_UTIL_INCLUDE_MSG_H_
#define SRC_UTIL_INCLUDE_MSG_H_

// clang-format off
#include <source_location>
#include <fmt/core.h>

#include <memory>
#include <string>

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
// clang-format on

namespace fika::log {

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

void msg_logger_init(const std::string &log_file = "app.log");

// Convenience macros to capture file/line automatically

inline std::string_view filename_only(std::string_view path) {
  auto pos = path.find_last_of("/\\");
  if (pos == std::string_view::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

template <typename... Ts>
struct log_debug {
  log_debug(const std::string &fmt, Ts &&...template_string,
            const std::source_location &loc = std::source_location::current()) {
    std::string msg =
        fmt::vformat(fmt, fmt::make_format_args(template_string...));
    BOOST_LOG_TRIVIAL(debug) << fmt::format(
        "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
  }
};

template <typename... Ts>
log_debug(const std::string &, Ts &&...) -> log_debug<Ts...>;

template <typename... Ts>
struct log_info {
  log_info(const std::string &fmt, Ts &&...template_string,
           const std::source_location &loc = std::source_location::current()) {
    std::string msg =
        fmt::vformat(fmt, fmt::make_format_args(template_string...));
    BOOST_LOG_TRIVIAL(info) << fmt::format(
        "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
  }
};

template <typename... Ts>
log_info(const std::string &, Ts &&...) -> log_info<Ts...>;

template <typename... Ts>
struct log_warn {
  log_warn(const std::string &fmt, Ts &&...template_string,
           const std::source_location &loc = std::source_location::current()) {
    std::string msg =
        fmt::vformat(fmt, fmt::make_format_args(template_string...));
    BOOST_LOG_TRIVIAL(warning) << fmt::format(
        "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
  }
};

template <typename... Ts>
log_warn(const std::string &, Ts &&...) -> log_warn<Ts...>;

template <typename... Ts>
struct log_error {
  log_error(const std::string &fmt, Ts &&...template_string,
            const std::source_location &loc = std::source_location::current()) {
    std::string msg =
        fmt::vformat(fmt, fmt::make_format_args(template_string...));
    BOOST_LOG_TRIVIAL(error) << fmt::format(
        "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
  }
};

template <typename... Ts>
log_error(const std::string &, Ts &&...) -> log_error<Ts...>;

template <typename... Ts>
struct log_critical {
  log_critical(
      const std::string &fmt, Ts &&...template_string,
      const std::source_location &loc = std::source_location::current()) {
    std::string msg =
        fmt::vformat(fmt, fmt::make_format_args(template_string...));
    BOOST_LOG_TRIVIAL(fatal) << fmt::format(
        "[{}:{}] {}", filename_only(loc.file_name()), loc.line(), msg);
  }
};

template <typename... Ts>
log_critical(const std::string &, Ts &&...) -> log_critical<Ts...>;

}  // namespace fika::log
#endif  // SRC_UTIL_INCLUDE_MSG_H_
