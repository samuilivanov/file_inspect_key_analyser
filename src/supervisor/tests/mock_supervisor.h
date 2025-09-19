#pragma once
#include <vector>

#include "supervisor_inter.h"
struct test_supervisor : public fika::supervisor_inter {
  std::vector<std::string> started;
  std::vector<std::string> stopped;
  std::vector<fika::CommandResponse> sent_messages;

  void start_workers(const std::string &service_name = "") override {
    started.push_back(service_name);
  }

  void stop_workers(const std::string &service_name = "") override {
    stopped.push_back(service_name);
  }
  void send_pong(const fika::CommandResponse &msg) override {
    sent_messages.push_back(msg);
  }
  void run() override {}
  void handle_command(fika::CommandType cmd_type,
                      const std::string &service_name = "") override {};
  void reset_queues() override {};
  void create_queues() override {};
  [[nodiscard]] std::vector<fika::worker_entity> &get_workers() override {};

  void monitor_once() override {};
  void register_commands() override {};
};