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

#include "magic_handle.h"

#include <memory>
#include <stdexcept>

#include "detectors/magic_api.h"

namespace fika {
magic_handle::magic_handle(std::unique_ptr<magic_api> api, int flags)
    : api_(std::move(api)), handle_(api_->open(flags)) {
  if (handle_ == nullptr) {
    throw std::runtime_error("Failed to open libmagic handle");
  }
  if (api_->load(handle_, nullptr) != 0) {
    throw std::runtime_error(api_->error(handle_));
  }
}

std::string magic_handle::detect(const std::string &filepath) {
  const char *res = api_->file(handle_, filepath.c_str());
  if (res == nullptr) {
    throw std::runtime_error(api_->error(handle_));
  }
  return res;
}

magic_handle::~magic_handle() {
  if (handle_ != nullptr) {
    api_->close(handle_);
  }
}

}  // namespace fika
