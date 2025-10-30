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

#include "pdf_parser.h"

#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-embedded-file.h>
#include <poppler/cpp/poppler-page.h>

#include "msg.h"

namespace fika {
void pdf_parser::parse(const file_job_shm &job) {
  metadata_.clear();
  text_.clear();
  embedded_files_.clear();
  warnings_.clear();

  auto filename = to_file_job(job).path;  // REFACTOR THIS
  std::unique_ptr<poppler::document> doc(
      poppler::document::load_from_file(filename));
  if (!doc) {
    throw std::runtime_error("Poppler failed to open PDF");
  }

  // Extract metadata
  static std::vector<std::string> keys = {"Title",        "Author",  "Subject",
                                          "Keywords",     "Creator", "Producer",
                                          "CreationDate", "ModDate"};

  for (auto &key : keys) {
    poppler::ustring value = doc->info_key(key);
    if (!value.empty()) {
      metadata_[key] = value.to_utf8().data();
      std::cout << key << ": " << value.to_utf8().data() << "\n";
    }
  }

  // Extract text page by page
  int num_pages = doc->pages();
  std::cout << "Text content:\n";
  for (int i = 0; i < num_pages; ++i) {
    std::unique_ptr<poppler::page> p(doc->create_page(i));
    if (p) {
      std::string text = p->text().to_utf8().data();
      text_.append(text).append("\n");
    }
  }

  auto attachments = doc->embedded_files();
  if (!attachments.empty()) {
    std::cout << "Embedded files:\n";
    for (const auto &att : attachments) {
      std::cout << att->name() << " (" << att->data().size() << " bytes)\n";
    }
  } else {
    std::cout << "No embedded files found.\n";
  }
}

[[nodiscard]] const char *pdf_parser::mime_type() const {
  return "application/pdf";
}

// Optional: retrieve parsed metadata in memory (for internal use or testing)
[[nodiscard]] std::map<std::string, std::string> pdf_parser::get_metadata()
    const {
  return metadata_;
}

// Optional: retrieve text in memory
[[nodiscard]] std::string pdf_parser::get_text() const { return text_; }

// Optional: retrieve names of embedded files
[[nodiscard]] std::vector<std::string> pdf_parser::get_embedded_files() const {
  return embedded_files_;
}

// Optional: return any warnings or fallback notes
[[nodiscard]] std::vector<std::string> pdf_parser::get_warnings() const {
  return warnings_;
}
}  // namespace fika