#include "parser_worker.h"
#include <iostream>

namespace fika {
parser_worker::parser_worker(std::shared_ptr<ipc_client> client,
                             std::shared_ptr<std::atomic<bool>> running,
                             std::shared_ptr<parser_registry> registry)
    : client_(std::move(client)), running_(std::move(running)),
      registry_(std::move(registry)) {}

void parser_worker::operator()() {
  file_job_shm job;
  while (running_->load()) {
    client_->receive_job(job);
    process_job(job);
    client_->send_result(job);
  }
}

void parser_worker::process_job(file_job_shm &job) {
  if (auto parser = registry_->find_parser(job.mime)) {
    parser->parse(job);

    job.status = Status::DONE;
  } else {
    job.status = Status::FAILED;
  }
}

} // namespace fika
