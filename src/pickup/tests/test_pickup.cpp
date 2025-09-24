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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "pickup.h"

TEST_CASE("pickup_service move_file and remove_file") {
  fika::pickup_service pickup;

  // Setup temp folder and files
  std::filesystem::path temp_dir =
      std::filesystem::temp_directory_path() / "pickup_test";
  std::filesystem::create_directories(temp_dir);

  std::filesystem::path file1 = temp_dir / "file1.txt";
  std::filesystem::path file2 = temp_dir / "file2.txt";

  // Create files
  std::ofstream(file1) << "test content";
  std::ofstream(file2) << "test content";

  SUBCASE("move_file moves file successfully") {
    std::filesystem::path dst = temp_dir / "moved_file1.txt";
    auto res = pickup.move_file(file1, dst);
    CHECK(res.has_value());
    CHECK_FALSE(std::filesystem::exists(file1));
    CHECK(std::filesystem::exists(dst));
  }

  SUBCASE("remove_file removes file successfully") {
    auto res = pickup.remove_file(file2);
    CHECK(res.has_value());
    CHECK_FALSE(std::filesystem::exists(file2));
  }

  SUBCASE("remove_file on non-existent file returns error") {
    auto res = pickup.remove_file(temp_dir / "non_existent.txt");
    CHECK_FALSE(res.has_value());
    CHECK(res.error() ==
          std::make_error_code(std::errc::no_such_file_or_directory));
  }

  // Cleanup
  std::filesystem::remove_all(temp_dir);
}