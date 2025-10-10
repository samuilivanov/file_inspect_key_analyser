#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-embedded-file.h>
#include <poppler/cpp/poppler-page.h>

#include <iostream>

#include "file_job.h"
#include "pdf_parser.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: pdf <file.pdf>\n";
    return 1;
  }

  std::string filename = argv[1];
  fika::pdf_parser parser;

  fika::file_job job;
  job.path = filename;
  fika::file_job_shm j_shm;
  j_shm = from_file_job(job);
  parser.parse(j_shm);
  std::cout << parser.get_text() << '\n';
  return 0;
}
