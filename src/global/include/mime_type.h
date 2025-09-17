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

#ifndef SRC_GLOBAL_INCLUDE_MIME_TYPE_H_
#define SRC_GLOBAL_INCLUDE_MIME_TYPE_H_

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace fika::mime {

// -------------------------
// Internal MIME type enum
// -------------------------
enum class Type : std::uint16_t {
  Unknown,
  MsOfficeWord,
  MsOfficeExcel,
  MsOfficePowerPoint,
  Pdf,
  ImageJpeg,
  ImagePng,
  TextPlain,
  // add more as needed
};

// -------------------------
// Metadata for each type
// -------------------------
struct MimeInfo {
  Type type;
  std::string_view internalName;  // canonical MIME string
  std::string_view description;   // human-readable
  std::string_view parserName;    // parser to use
};

// -------------------------
// Detector MIME -> internal Type
// -------------------------
struct DetectorMapEntry {
  std::string_view detectorMime;
  Type type;
};

// -------------------------
// constexpr tables
// -------------------------
constexpr std::array<MimeInfo, 7> mimeInfos = {{
    {Type::MsOfficeWord, "application/ms-word", "Microsoft Word Document",
     "WordParser"},
    {Type::MsOfficeExcel, "application/ms-excel", "Microsoft Excel Spreadsheet",
     "ExcelParser"},
    {Type::MsOfficePowerPoint, "application/ms-powerpoint",
     "Microsoft PowerPoint Presentation", "PowerPointParser"},
    {Type::Pdf, "application/pdf", "PDF Document", "PdfParser"},
    {Type::ImageJpeg, "image/jpeg", "JPEG Image", "ImageParser"},
    {Type::ImagePng, "image/png", "PNG Image", "ImageParser"},
    {Type::TextPlain, "text/plain", "Plain Text File", "TextParser"},
}};

constexpr std::array<DetectorMapEntry, 6> detectorMap = {{
    {"application/msword", Type::MsOfficeWord},
    {"application/vnd.ms-word", Type::MsOfficeWord},
    {"application/vnd.openxmlformats-officedocument.wordprocessingml.document",
     Type::MsOfficeWord},
    {"application/vnd.ms-excel", Type::MsOfficeExcel},
    {"application/pdf", Type::Pdf},
    {"image/jpeg", Type::ImageJpeg},
}};

// -------------------------
// Detection result
// -------------------------
struct DetectionResult {
  std::string mime_type;
  std::string source;
  double confidence{1.0};
};

// -------------------------
// Enriched result
// -------------------------
struct EnrichedResult {
  DetectionResult base;
  MimeInfo info;
};

static constexpr Type map_type(std::string_view detectedMime) {
  for (auto const& entry : detectorMap) {
    if (entry.detectorMime == detectedMime) return entry.type;
  }
  return Type::Unknown;
}

// constexpr lookup of metadata from Type
static constexpr MimeInfo map_info(Type type) {
  for (auto const& info : mimeInfos) {
    if (info.type == type) return info;
  }
  // fallback unknown
  return {Type::Unknown, "unknown", "Unknown file type", "None"};
}

// Enrich a detection result
static EnrichedResult enrich(const DetectionResult& det) {
  Type t = map_type(det.mime_type);
  MimeInfo info = map_info(t);
  return {det, info};
}

}  // namespace fika::mime

#endif  // SRC_GLOBAL_INCLUDE_MIME_TYPE_H_
