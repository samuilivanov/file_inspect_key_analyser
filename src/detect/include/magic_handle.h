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

#ifndef MAGIC_HANDLE_H
#define MAGIC_HANDLE_H
#include "detectors/file_detector.h"
#include "detectors/magic_api.h"
#include <magic.h>
#include <memory>
#include <string>

namespace fika {

class magic_handle : public file_detector {
private:
  std::unique_ptr<magic_api> api_;
  void *handle_;

public:
  explicit magic_handle(std::unique_ptr<magic_api> api,
                        int flags = MAGIC_MIME_TYPE);
  ~magic_handle();
  std::string detect(const std::string &filepath) override;
  std::string name() const override {
    return "libmagic";
  } // source identifier, e.g., "libmagic"
};

} // namespace fika

#endif