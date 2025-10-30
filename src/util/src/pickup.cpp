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

#include "pickup.h"

namespace fika {
std::expected<void, std::error_code> pickup_service::move_file(
    const std::filesystem::path &src, const std::filesystem::path &dst) {
  std::error_code ec;
  std::filesystem::rename(src, dst, ec);
  if (ec) {
    return std::unexpected(ec);
  }
  return {};
}

std::expected<void, std::error_code> pickup_service::remove_file(
    const std::filesystem::path &file) {
  std::error_code ec;
  if (!std::filesystem::remove(file, ec)) {
    if (!ec) {
      // File didn't exist
      return std::unexpected(
          std::make_error_code(std::errc::no_such_file_or_directory));
    }
    return std::unexpected(ec);
  }
  return {};
}
}  // namespace fika