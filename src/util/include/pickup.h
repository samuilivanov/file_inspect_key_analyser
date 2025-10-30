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

#ifndef SRC_PICKUP_INCLUDE_PICKUP_H_
#define SRC_PICKUP_INCLUDE_PICKUP_H_

#include <expected>
#include <filesystem>
#include <system_error>

namespace fika {

struct pickup_interface {
  virtual ~pickup_interface() = default;

  // Move a single file from src to dst
  virtual std::expected<void, std::error_code> move_file(
      const std::filesystem::path &src, const std::filesystem::path &dst) = 0;

  // Remove a single file
  virtual std::expected<void, std::error_code> remove_file(
      const std::filesystem::path &file) = 0;
};

// ---------------------- Implementation ----------------------
class pickup_service : public pickup_interface {
 public:
  std::expected<void, std::error_code> move_file(
      const std::filesystem::path &src,
      const std::filesystem::path &dst) override;
  std::expected<void, std::error_code> remove_file(
      const std::filesystem::path &file) override;
};
}  // namespace fika

#endif  // SRC_PICKUP_INCLUDE_PICKUP_H_
