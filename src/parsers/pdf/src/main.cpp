#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-embedded-file.h>
#include <poppler/cpp/poppler-page.h>

#include <iostream>
// #include <memory>
// #include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: pdf <file.pdf>\n";
    return 1;
  }

  std::string filename = argv[1];

  // Load PDF document
  std::unique_ptr<poppler::document> doc(
      poppler::document::load_from_file(filename));
  if (!doc) {
    std::cerr << "Failed to open PDF: " << filename << "\n";
    return 1;
  }

  // Extract metadata
  std::cout << "Metadata:\n";
  std::vector<std::string> keys = {"Title",        "Author",  "Subject",
                                   "Keywords",     "Creator", "Producer",
                                   "CreationDate", "ModDate"};
  for (auto &key : keys) {
    poppler::ustring value = doc->info_key(key);
    if (!value.empty())
      std::cout << key << ": " << value.to_utf8().data() << "\n";
  }
  std::cout << "------------------------\n";

  // Extract text page by page
  int num_pages = doc->pages();
  std::cout << "Text content:\n";
  for (int i = 0; i < num_pages; ++i) {
    std::unique_ptr<poppler::page> p(doc->create_page(i));
    if (p) {
      std::string text = p->text().to_utf8().data();
      std::cout << "--- Page " << i + 1 << " ---\n";
      std::cout << text << "\n";
    }
  }
  std::cout << "------------------------\n";

  // List embedded files (attachments)
  auto attachments = doc->embedded_files();
  if (!attachments.empty()) {
    std::cout << "Embedded files:\n";
    for (const auto &att : attachments) {
      std::cout << att->name() << " (" << att->data().size() << " bytes)\n";
    }
  } else {
    std::cout << "No embedded files found.\n";
  }

  return 0;
}
