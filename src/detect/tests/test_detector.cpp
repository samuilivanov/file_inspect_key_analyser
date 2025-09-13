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

#include "detector.h"
#include "detectors/file_detector.h"
#include <doctest/doctest.h>
#include <memory>
#include <string>
#include <vector>

// Mock detector
struct mock_detector : public fika::file_detector {
  mock_detector(std::string name, std::string type)
      : name_(std::move(name)), type_(std::move(type)) {}
  std::string detect(const std::string &) override { return type_; }
  std::string name() const override { return name_; }

private:
  std::string name_;
  std::string type_;
};

TEST_CASE("file_detector::name() returns correct detector name") {
  mock_detector md("magic", "text/plain");

  CHECK(md.name() == "magic");
}

TEST_CASE("detector returns correct source from name()") {
  std::vector<std::unique_ptr<fika::file_detector>> dets;
  dets.push_back(
      std::make_unique<mock_detector>("magic", "application/octet-stream"));
  dets.push_back(std::make_unique<mock_detector>("custom", "image/png"));

  fika::detect::detector det(std::move(dets));
  auto res = det.detect_file("file.png");

  // The detector source comes from name() of the winning detector
  CHECK(res.source == "custom");
}

TEST_CASE("Single detector returns type") {
  std::vector<std::unique_ptr<fika::file_detector>> dets;
  dets.push_back(std::make_unique<mock_detector>("magic", "text/plain"));
  fika::detect::detector det(std::move(dets));

  auto res = det.detect_file("file.txt");
  CHECK(res.mime_type == "text/plain");
  CHECK(res.source == "magic");
  CHECK(res.confidence == doctest::Approx(1.0));
}

TEST_CASE("Fallback to next detector") {
  std::vector<std::unique_ptr<fika::file_detector>> dets;
  dets.push_back(
      std::make_unique<mock_detector>("magic", "application/octet-stream"));
  dets.push_back(std::make_unique<mock_detector>("custom", "image/png"));
  fika::detect::detector det(std::move(dets));

  auto res = det.detect_file("file.png");
  CHECK(res.mime_type == "image/png");
  CHECK(res.source == "custom");
}

TEST_CASE("All detectors fail returns octet-stream") {
  std::vector<std::unique_ptr<fika::file_detector>> dets;
  dets.push_back(
      std::make_unique<mock_detector>("magic", "application/octet-stream"));
  dets.push_back(std::make_unique<mock_detector>("custom", ""));
  fika::detect::detector det(std::move(dets));

  auto res = det.detect_file("file.unknown");
  CHECK(res.mime_type == "application/octet-stream");
  CHECK(res.source == "none");
}
