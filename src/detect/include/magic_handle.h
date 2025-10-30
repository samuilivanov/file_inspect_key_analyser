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

#ifndef SRC_DETECT_INCLUDE_MAGIC_HANDLE_H_
#define SRC_DETECT_INCLUDE_MAGIC_HANDLE_H_
#include <magic.h>

#include <memory>
#include <string>

#include "detectors/file_detector.h"
#include "detectors/magic_api.h"

namespace fika {

class magic_handle : public file_detector {
 private:
  std::unique_ptr<magic_api> api_;
  void* handle_{nullptr};

 public:
  explicit magic_handle(std::unique_ptr<magic_api> api,
                        int flags = MAGIC_MIME_TYPE);
  ~magic_handle() override;
  magic_handle(const magic_handle& other) = delete;
  magic_handle& operator=(const magic_handle&) = delete;
  magic_handle(magic_handle&& other) = delete;
  magic_handle& operator=(magic_handle&& other) = delete;
  std::string detect(const std::string& filepath) override;
  [[nodiscard]] std::string name() const override {
    return "libmagic";
  }  // source identifier, e.g., "libmagic"
};

}  // namespace fika

#endif  // SRC_DETECT_INCLUDE_MAGIC_HANDLE_H_
