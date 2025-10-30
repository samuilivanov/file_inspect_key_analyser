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

#ifndef SRC_PARSE_INCLUDE_PARSER_H_
#define SRC_PARSE_INCLUDE_PARSER_H_

#include <map>
#include <vector>

#include "file_job.h"

namespace fika {

// Abstract parser interface
class parser {
 public:
  parser() = default;
  virtual ~parser() = default;
  parser(const parser &) = delete;
  parser &operator=(const parser &) = delete;
  parser(parser &&) = delete;
  parser &operator=(parser &&) = delete;

  // Parse file and update the job with results
  virtual void parse(const file_job_shm &job) = 0;

  // Returns mime type this parser can handle
  [[nodiscard]] virtual const char *mime_type() const = 0;

  // Optional: retrieve parsed metadata in memory (for internal use or testing)
  [[nodiscard]] virtual std::map<std::string, std::string> get_metadata()
      const = 0;

  // Optional: retrieve text in memory
  [[nodiscard]] virtual std::string get_text() const = 0;

  // Optional: retrieve names of embedded files
  [[nodiscard]] virtual std::vector<std::string> get_embedded_files() const = 0;

  // Optional: return any warnings or fallback notes
  [[nodiscard]] virtual std::vector<std::string> get_warnings() const = 0;
};

}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSER_H_
