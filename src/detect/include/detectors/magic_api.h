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

#ifndef MAGIC_API_H
#define MAGIC_API_H
#include <magic.h>
#include <stdexcept>
#include <string>
namespace fika {

struct magic_api {
  virtual void *open(int flags) = 0;
  virtual int load(void *handle, const char *file) = 0;
  virtual const char *file(void *handle, const char *path) = 0;
  virtual void close(void *handle) = 0;
  virtual const char *error(void *handle) = 0;
  virtual ~magic_api() = default;
};

// Real libmagic adapter
struct libmagic_api : public magic_api {
  void *open(int flags) override { return magic_open(flags); }

  int load(void *handle, const char *file) override {
    return magic_load(static_cast<magic_t>(handle), file);
  }

  const char *file(void *handle, const char *path) override {
    return magic_file(static_cast<magic_t>(handle), path);
  }

  void close(void *handle) override {
    magic_close(static_cast<magic_t>(handle));
  }

  const char *error(void *handle) override {
    return magic_error(static_cast<magic_t>(handle));
  }
};
} // namespace fika

#endif