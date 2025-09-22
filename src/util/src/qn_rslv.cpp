#include "qn_rslv.h"

namespace fika::util {
std::optional<std::pair<std::string, std::string>> parse_direct_queue(
    const std::string &queue_name) {
  const std::string prefix = "fika.direct.";
  const std::string sep = ".to.";

  if (queue_name.rfind(prefix, 0) != 0) {
    return std::nullopt;  // not matching
  }

  auto pos = queue_name.find(sep, prefix.size());
  if (pos == std::string::npos) {
    return std::nullopt;  // missing separator
  }

  std::string sender = queue_name.substr(prefix.size(), pos - prefix.size());
  std::string receiver = queue_name.substr(pos + sep.size());

  if (sender.empty() || receiver.empty()) {
    return std::nullopt;
  }

  return std::make_pair(sender, receiver);
}

std::optional<std::string> parse_inbox_queue(const std::string &queue_name) {
  const std::string prefix = "fika.inbox.";

  if (queue_name.rfind(prefix, 0) != 0) {
    return std::nullopt;  // not matching
  }

  std::string binary = queue_name.substr(prefix.size());
  if (binary.empty()) {
    return std::nullopt;  // no binary name
  }

  return binary;
}

}  // namespace fika::util