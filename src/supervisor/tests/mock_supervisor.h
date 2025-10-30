/*
 * This file is part of file_ingest_key_analyser (FIKA).
 *
 * Copyright [2025] Samuil Ivanov
 *
 * file_ingest_key_analyser (FIKA) is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License.
 *
 * file_ingest_key_analyser (FIKA) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with file_ingest_key_analyser (FIKA).  If not, see
 * <http://www.gnu.org/licenses/>.
 */

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