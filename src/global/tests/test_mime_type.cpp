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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <array>
#include <string>
#include <string_view>

#include "../include/mime_type.h"

using namespace fika::mime;

TEST_CASE("map_type maps known detector MIME strings to Type") {
  CHECK(map_type("application/msword") == Type::MsOfficeWord);
  CHECK(map_type("application/vnd.ms-word") == Type::MsOfficeWord);
  CHECK(
      map_type("application/"
               "vnd.openxmlformats-officedocument.wordprocessingml.document") ==
      Type::MsOfficeWord);

  CHECK(map_type("application/vnd.ms-excel") == Type::MsOfficeExcel);

  CHECK(map_type("application/pdf") == Type::Pdf);

  CHECK(map_type("image/jpeg") == Type::ImageJpeg);
}

TEST_CASE("map_type returns Unknown for unmapped MIME strings") {
  CHECK(map_type("application/unknown") == Type::Unknown);
  CHECK(map_type("") == Type::Unknown);
  CHECK(map_type("text/html") == Type::Unknown);
}

TEST_CASE("map_info returns correct MimeInfo for known types") {
  {
    auto info = map_info(Type::MsOfficeWord);
    CHECK(info.type == Type::MsOfficeWord);
    CHECK(info.internalName == "application/ms-word");
    CHECK(info.description == "Microsoft Word Document");
    CHECK(info.parserName == "WordParser");
  }

  {
    auto info = map_info(Type::Pdf);
    CHECK(info.type == Type::Pdf);
    CHECK(info.internalName == "application/pdf");
    CHECK(info.description == "PDF Document");
    CHECK(info.parserName == "PdfParser");
  }

  {
    auto info = map_info(Type::ImageJpeg);
    CHECK(info.type == Type::ImageJpeg);
    CHECK(info.internalName == "image/jpeg");
    CHECK(info.description == "JPEG Image");
    CHECK(info.parserName == "ImageParser");
  }
}

TEST_CASE("map_info returns fallback for unknown Type") {
  auto info = map_info(Type::Unknown);
  CHECK(info.type == Type::Unknown);
  CHECK(info.internalName == "unknown");
  CHECK(info.description == "Unknown file type");
  CHECK(info.parserName == "None");
}

TEST_CASE("enrich combines DetectionResult with correct MimeInfo") {
  DetectionResult det{"application/pdf", "sniffer", 0.95};
  auto enriched = enrich(det);

  CHECK(enriched.base.mime_type == "application/pdf");
  CHECK(enriched.base.source == "sniffer");
  CHECK(enriched.base.confidence == doctest::Approx(0.95));

  CHECK(enriched.info.type == Type::Pdf);
  CHECK(enriched.info.internalName == "application/pdf");
  CHECK(enriched.info.parserName == "PdfParser");
}

TEST_CASE("enrich handles unknown MIME gracefully") {
  DetectionResult det{"application/foobar", "sniffer", 0.42};
  auto enriched = enrich(det);

  CHECK(enriched.base.mime_type == "application/foobar");
  CHECK(enriched.base.source == "sniffer");
  CHECK(enriched.base.confidence == doctest::Approx(0.42));

  CHECK(enriched.info.type == Type::Unknown);
  CHECK(enriched.info.internalName == "unknown");
  CHECK(enriched.info.description == "Unknown file type");
  CHECK(enriched.info.parserName == "None");
}
