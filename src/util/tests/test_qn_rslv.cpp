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
#include "doctest/doctest.h"
#include "qn_rslv.h"

TEST_CASE("direct queue name is correct") {
  auto q = get_direct_queue(fika::util::make_sender("auth"),
                            fika::util::make_receiver("billing"));
  CHECK(q == "fika.direct.auth.to.billing");

  auto q2 = get_direct_queue(fika::util::make_sender("worker"),
                             fika::util::make_receiver("db"));
  CHECK(q2 == "fika.direct.worker.to.db");
}

TEST_CASE("inbox queue name is correct") {
  auto q = get_inbox_queue(fika::util::make_receiver("billing"));
  CHECK(q == "fika.inbox.billing");

  auto q2 = get_inbox_queue(fika::util::make_receiver("logger"));
  CHECK(q2 == "fika.inbox.logger");
}

TEST_CASE("empty names are allowed but produce valid strings") {
  auto q1 = get_direct_queue(fika::util::make_sender(""),
                             fika::util::make_receiver("x"));
  CHECK(q1 == "fika.direct..to.x");

  auto q2 = get_inbox_queue(fika::util::make_receiver(""));
  CHECK(q2 == "fika.inbox.");
}

TEST_CASE("parse_direct_queue: valid inputs") {
  auto q = fika::util::parse_direct_queue("fika.direct.mailer.to.worker");
  REQUIRE(q.has_value());
  CHECK(q->first == "mailer");
  CHECK(q->second == "worker");

  q = fika::util::parse_direct_queue("fika.direct.a.to.b");
  REQUIRE(q.has_value());
  CHECK(q->first == "a");
  CHECK(q->second == "b");
}

TEST_CASE("parse_direct_queue: invalid inputs") {
  CHECK(!fika::util::parse_direct_queue("fika.direct.mailer-to-worker")
             .has_value());
  CHECK(!fika::util::parse_direct_queue("fika.direct.queue._to.worker")
             .has_value());
  CHECK(!fika::util::parse_direct_queue("fika.direct.mailer.to.").has_value());
  CHECK(!fika::util::parse_direct_queue("wrong_prefix.mailer.to.worker")
             .has_value());
}

TEST_CASE("parse_inbox_queue: valid inputs") {
  auto q = fika::util::parse_inbox_queue("fika.inbox.worker");
  REQUIRE(q.has_value());
  CHECK(*q == "worker");

  q = fika::util::parse_inbox_queue("fika.inbox.binary123");
  REQUIRE(q.has_value());
  CHECK(*q == "binary123");
}

TEST_CASE("parse_inbox_queue: invalid inputs") {
  CHECK(!fika::util::parse_inbox_queue("fika.inbox.").has_value());
  CHECK(!fika::util::parse_inbox_queue("wrong.prefix.worker").has_value());
  CHECK(!fika::util::parse_inbox_queue("").has_value());
}