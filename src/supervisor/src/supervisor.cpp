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

#include "supervisor.h"

// clang-format off
#include <chrono>

#include "cmd.h"
#include "cmd_def.h"
#include "commands.h"
#include "config.h"
#include "config_loader.hpp"
#include "msg.h"
#include "worker_configs.h"

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
// clang-format on

namespace {

std::map<std::string, std::string> binary_to_queue = {
    {"detect", DETECT_MESSAGE_QUEUE.data()},
    {"parse", PARSE_MESSAGE_QUEUE.data()},
    {"mq", QM_MESSAGE_QUEUE.data()}};

}  // namespace

namespace fika {

supervisor::supervisor(std::vector<worker_factory_t> factories,
                       std::vector<queue_descriptor> queues,
                       std::shared_ptr<ipc_queue_manager> queue_mgr)
    : queues_(queues), queue_mgr_(queue_mgr) {
  for (const auto &f : factories)
    workers_.push_back({f(), WorkerState::Stopped});
}

void supervisor::start_workers(const std::string &service_name) {
  for (auto &w : workers_) {
    if (service_name.empty() || w.w->name() == service_name) {
      w.w->start();
      w.state = WorkerState::Running;
    }
  }
}

void supervisor::monitor_once() {
  for (auto &w : workers_) {
    if (!w.w->is_alive() && w.state == WorkerState::Running) {
      log::log_info("Restarting worker");
      w.w->restart();
    }
  }
}

void supervisor::stop_workers(const std::string &service_name) {
  if (service_name.empty()) {
    for (auto &w : workers_) {
      w.w->stop();
      w.state = WorkerState::Stopped;
    }
    reset_queues();
  } else {
    for (auto &w : workers_) {
      if (w.w->name() == service_name) {
        // 1. enqueue poison pill
        queue_mgr_->send_stop_job(binary_to_queue[service_name]);

        // 2. wait for graceful shutdown
        w.w->wait();

        w.state = WorkerState::Stopped;
      }
    }
  }
}

void supervisor::reset_queues() {
  for (const auto &q : queues_) {
    queue_mgr_->remove(q.name);
    log::log_info("Removed stale queue: {}", q.name);
  }
}

void supervisor::create_queues() {
  for (const auto &q : queues_) {
    queue_mgr_->create(q.name, q.max_messages, q.message_size);
    std::cout << "Created queue: " << q.name
              << " (max_messages=" << q.max_messages
              << ", message_size=" << q.message_size << ")\n";
  }
}

void supervisor::register_commands() {
  commands_[CommandType::Start] = std::make_unique<detail::start_command>();
  commands_[CommandType::Stop] = std::make_unique<detail::stop_command>();
  commands_[CommandType::Restart] = std::make_unique<detail::restart_command>();
}

void supervisor::handle_command(CommandType cmd_type,
                                const std::string &service_name) {
  auto it = commands_.find(cmd_type);
  if (it != commands_.end()) {
    it->second->execute(*this, service_name);
  } else {
    log::log_info("Unknown command");
  }
}

void supervisor::run() {
  boost::interprocess::message_queue mq(boost::interprocess::open_or_create,
                                        "fika_supervisor_mq", 100,
                                        sizeof(CommandMessage));

  while (true) {
    CommandMessage msg;
    std::size_t recv_size;
    unsigned int priority;

    if (mq.try_receive(&msg, sizeof(msg), recv_size, priority)) {
      handle_command(msg.cmd, msg.service());
    }
    monitor_once();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

}  // namespace fika
