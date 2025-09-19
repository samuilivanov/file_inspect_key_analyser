#pragma once
#include <string>
#include <vector>

#include "commands.h"
#include "worker.h"

namespace fika {

struct supervisor_inter {
  supervisor_inter() = default;
  virtual ~supervisor_inter() = default;
  supervisor_inter(const supervisor_inter &) = delete;
  supervisor_inter(supervisor_inter &&) = delete;
  supervisor_inter &operator=(const supervisor_inter &) = delete;
  supervisor_inter &operator=(supervisor_inter &&) = delete;

  virtual void run() = 0;
  virtual void start_workers(const std::string &service_name = "") = 0;
  virtual void stop_workers(const std::string &service_name = "") = 0;
  virtual void handle_command(CommandType cmd_type,
                              const std::string &service_name = "") = 0;
  virtual void reset_queues() = 0;
  virtual void create_queues() = 0;
  [[nodiscard]] virtual std::vector<worker_entity> &get_workers() = 0;

  virtual void monitor_once() = 0;
  virtual void send_pong(const CommandResponse &msg) = 0;

  virtual void register_commands() = 0;
};
}  // namespace fika