#ifndef PDF_PARSER_H
#define PDF_PARSER_H

#include "parser.h"

namespace fika {

class pdf_parser : public parser {
private:
  /* data */
public:
  pdf_parser(/* args */);
  ~pdf_parser();
  void parse(file_job_shm &job) override;

  const char *mime_type() const override;
};

void pdf_parser::parse(file_job_shm &job) { std::cout << "in pdf parse"; }

const char *pdf_parser::mime_type() const { return "pdf"; }

pdf_parser::pdf_parser(/* args */) {}

pdf_parser::~pdf_parser() {}
} // namespace fika

#endif