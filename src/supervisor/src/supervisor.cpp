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
#include "qn_rslv.h"

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <exception>
// clang-format on

namespace {

const std::map<std::string, std::string> binary_to_queue = {
    {"detect",
     fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                  fika::util::make_receiver("detect"))},
    {"parse", fika::util::get_direct_queue(fika::util::make_sender("qm"),
                                           fika::util::make_receiver("parse"))},
    {"qm", fika::util::get_inbox_queue(fika::util::make_receiver("qm"))}};

}  // namespace

namespace fika {

supervisor::supervisor(const std::vector<worker_factory_t> &factories,
                       std::vector<queue_descriptor> queues,
                       std::shared_ptr<ipc_queue_manager> queue_mgr)
    : queues_(std::move(queues)), queue_mgr_(std::move(queue_mgr)) {
  mq_receive = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_or_create, "fika_supervisor_mq", 100,
      sizeof(CommandMessage));
  mq_send = std::make_unique<boost::interprocess::message_queue>(
      boost::interprocess::open_or_create, "supervisor_fika_mq", 100,
      sizeof(CommandResponse));

  for (const auto &f : factories) {
    workers_.push_back({f(), WorkerState::Stopped});
  }
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
  for (auto &w : workers_) {
    if (w.w->name() == service_name || service_name.empty()) {
      // 1. enqueue poison pill
      auto queue = binary_to_queue.find(w.w->name());
      if (queue != binary_to_queue.end()) {
        try {
          queue_mgr_->send_stop_job(queue->second);
          log::log_info("Sending poison pill to {} on message queue {}",
                        queue->first, queue->second);
        } catch (const std::exception &e) {
          log::log_info("Exception {}", e.what());
        }
      } else {
        log::log_warn(
            "Message queue for poison pill for service {} not found. "
            "Continue...",
            service_name);
        continue;
      }

      // 2. wait for graceful shutdown
      w.w->wait();

      w.state = WorkerState::Stopped;
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
    fika::log::log_info("Created queue: {} (max_messages={}, message_size={}",
                        q.name, q.max_messages, q.message_size);
  }
}

void supervisor::register_commands() {
  commands_[CommandType::Start] = std::make_unique<detail::start_command>();
  commands_[CommandType::Stop] = std::make_unique<detail::stop_command>();
  commands_[CommandType::Restart] = std::make_unique<detail::restart_command>();
  commands_[CommandType::Ping] = std::make_unique<detail::ping_command>();
}

void supervisor::handle_command(CommandType cmd_type,
                                const std::string &service_name) {
  auto it = commands_.find(cmd_type);
  if (it != commands_.end()) {
    it->second->execute(this, service_name);
  } else {
    log::log_info("Unknown command");
  }
}

void supervisor::run() {
  while (true) {
    CommandMessage msg;
    std::size_t recv_size{};
    unsigned int priority{};

    if (mq_receive->try_receive(&msg, sizeof(msg), recv_size, priority)) {
      handle_command(msg.cmd_, msg.service());
    }
    monitor_once();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

void supervisor::send_pong(const CommandResponse &msg) {
  mq_send->send(&msg, sizeof(CommandResponse), 0);
}

}  // namespace fika
