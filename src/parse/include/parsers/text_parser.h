#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H

#include "parser.h"

namespace fika {

class text_parser : public parser {
private:
  /* data */
public:
  text_parser(/* args */);
  ~text_parser();
  void parse(file_job_shm &job) override;

  const char *mime_type() const override;
};
void text_parser::parse(file_job_shm &job) { std::cout << "in txt parse"; }

const char *text_parser::mime_type() const { return "txt"; }
text_parser::text_parser(/* args */) {}

text_parser::~text_parser() {}
} // namespace fika

#endif