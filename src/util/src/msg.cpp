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

#include "msg.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

namespace fika::log {
void msg_logger_init(const std::string &log_file) {
  static std::once_flag flag;
  std::call_once(flag, [log_file] {
    namespace logging = boost::log;
    namespace sinks = boost::log::sinks;
    namespace expr = boost::log::expressions;
    namespace attrs = boost::log::attributes;

    logging::add_common_attributes();
    boost::shared_ptr<logging::core> core = logging::core::get();
    core->add_global_attribute("ProcessName", attrs::current_process_name());

    // --- File sink ---
    typedef sinks::text_file_backend backend_t;
    boost::shared_ptr<backend_t> backend = boost::make_shared<backend_t>(
        boost::log::keywords::file_name = log_file,
        boost::log::keywords::open_mode = std::ios::app, // append mode
        boost::log::keywords::auto_flush = true);

    typedef sinks::synchronous_sink<backend_t> sink_t;
    boost::shared_ptr<sink_t> file_sink = boost::make_shared<sink_t>(backend);

    // Formatter: [timestamp] [severity] [binary] [file:line] message
    file_sink->set_formatter(
        expr::stream << "["
                     << expr::format_date_time<boost::posix_time::ptime>(
                            "TimeStamp", "%Y-%m-%d %H:%M:%S")
                     << "] [" << logging::trivial::severity << "] ["
                     << expr::attr<std::string>("ProcessName") << "] "
                     << expr::smessage);

    logging::core::get()->add_sink(file_sink);

    // --- Console sink ---
    typedef sinks::synchronous_sink<sinks::text_ostream_backend> console_sink_t;
    auto console_backend = boost::make_shared<sinks::text_ostream_backend>();
    console_backend->add_stream(
        boost::shared_ptr<std::ostream>(&std::clog, [](std::ostream *) {}));
    auto console_sink = boost::make_shared<console_sink_t>(console_backend);
    console_sink->set_formatter(
        expr::stream << "["
                     << expr::format_date_time<boost::posix_time::ptime>(
                            "TimeStamp", "%Y-%m-%d %H:%M:%S")
                     << "] [" << logging::trivial::severity << "] ["
                     << expr::attr<std::string>("ProcessName") << "] "
                     << expr::smessage);

    logging::core::get()->add_sink(console_sink);
  });
}

} // namespace fika::log