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

#ifndef SRC_PARSE_INCLUDE_PARSERS_IMG_PARSER_H_
#define SRC_PARSE_INCLUDE_PARSERS_IMG_PARSER_H_

#include <iostream>

#include "msg.h"
#include "parser.h"
namespace fika {

class img_parser : public parser {
 private:
  /* data */
 public:
  img_parser(/* args */);
  ~img_parser();
  void parse(file_job_shm &job) override;

  const char *mime_type() const override;
};
void img_parser::parse(file_job_shm &job) { log::log_info("test img"); }

const char *img_parser::mime_type() const { return "img"; }
img_parser::img_parser(/* args */) {}

img_parser::~img_parser() {}
}  // namespace fika

#endif  // SRC_PARSE_INCLUDE_PARSERS_IMG_PARSER_H_
