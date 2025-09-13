/*
 * This file is part of Fika.
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

#ifndef WORKER_H
#define WORKER_H

#include "worker_configs.h"
#include <boost/process.hpp>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

namespace fika {

struct child_process {
  virtual ~child_process() = default;
  virtual bool running() const = 0;
  virtual void terminate() = 0;
  virtual void wait() = 0;
  virtual std::string name() const = 0;
};

class boost_child_process : public child_process {
public:
  boost_child_process(const std::string &path,
                      const std::vector<std::string> &args)
      : proc_(path, boost::process::args(args)),
        proc_name(std::filesystem::path(path).filename()) {}

  bool running() const override { return proc_.running(); }

  void terminate() override {
    if (proc_.running()) {
      proc_.terminate();
    }
  }

  void wait() override { proc_.wait(); }
  std::string name() const override { return proc_name; }

private:
  mutable boost::process::child proc_;
  std::string proc_name;
};

class worker {
public:
  using process_factory_t = std::function<std::unique_ptr<child_process>()>;

  explicit worker(process_factory_t factory);
  void start();
  void stop();
  bool is_alive() const;
  void restart();
  std::string name() const;

private:
  std::chrono::steady_clock::time_point last_heartbeat_;
  std::unique_ptr<child_process> process_;
  process_factory_t factory_;
};

enum class WorkerState { Running, Stopped };

struct worker_entity {
  std::unique_ptr<worker> w;
  WorkerState state;
};
} // namespace fika

#endif