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

#ifndef SRC_PARSE_INCLUDE_PARSERS_TEXT_PARSER_H_
#define SRC_PARSE_INCLUDE_PARSERS_TEXT_PARSER_H_

#include "parser.h"

namespace fika {

class text_parser : public parser {
 private:
  /* data */
 public:
  text_parser(/* args */);
  ~text_parser();
  void parse(const file_job_shm &job) override;

  const char *mime_type() const override;
};
void text_parser::parse(const file_job_shm &job) {
  std::cout << "in txt parse";
}

const char *text_parser::mime_type() const { return "txt"; }
text_parser::text_parser(/* args */) {}

text_parser::~text_parser() {}
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSERS_TEXT_PARSER_H_
