#ifndef IMG_PARSER_H
#define IMG_PARSER_H

#include "msg.h"
#include "parser.h"
#include <iostream>
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
void img_parser::parse(file_job_shm &job) {
  
  msg_logger::log_info("test img"); 

}

const char *img_parser::mime_type() const { return "img"; }
img_parser::img_parser(/* args */) {}

img_parser::~img_parser() {}
} // namespace fika

#endif