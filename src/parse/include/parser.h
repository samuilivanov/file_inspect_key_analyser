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

#ifndef PARSER_H
#define PARSER_H

#include "file_job.h"

namespace fika {

// Abstract parser interface
class parser {
public:
  virtual ~parser() = default;

  // Parse file and update the job with results
  virtual void parse(file_job_shm &job) = 0;

  // Returns mime type this parser can handle
  virtual const char *mime_type() const = 0;
};

} // namespace fika

#endif