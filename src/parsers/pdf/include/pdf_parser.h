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

#ifndef SRC_PARSE_INCLUDE_PDF_MODULE_PDF_PARSER_H_
#define SRC_PARSE_INCLUDE_PDF_MODULE_PDF_PARSER_H_

#include "parser.h"

namespace fika {

class pdf_parser : public parser {
 private:
  std::map<std::string, std::string> metadata_;
  std::string text_;
  std::vector<std::string> embedded_files_;
  std::vector<std::string> warnings_;

 public:
  void parse(const file_job_shm &job) override;

  [[nodiscard]] const char *mime_type() const override;

  // Optional: retrieve parsed metadata in memory (for internal use or testing)
  [[nodiscard]] std::map<std::string, std::string> get_metadata()
      const override;

  // Optional: retrieve text in memory
  [[nodiscard]] std::string get_text() const override;

  // Optional: retrieve names of embedded files
  [[nodiscard]] std::vector<std::string> get_embedded_files() const override;

  // Optional: return any warnings or fallback notes
  [[nodiscard]] std::vector<std::string> get_warnings() const override;
};
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PDF_MODULE_PDF_PARSER_H_
