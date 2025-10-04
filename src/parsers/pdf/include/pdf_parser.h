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

#include <iostream>

#include "parser.h"

namespace fika {

class pdf_parser : public parser {
 private:
  /* data */
 public:
  pdf_parser(/* args */) = default;
  ~pdf_parser() = default;
  void parse(const file_job_shm &job) override;

  [[nodiscard]] const char *mime_type() const override;
};

void pdf_parser::parse(const file_job_shm &job) { std::cout << "in pdf parse"; }

[[nodiscard]] const char *pdf_parser::mime_type() const { return "pdf"; }

}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PDF_MODULE_PDF_PARSER_H_
