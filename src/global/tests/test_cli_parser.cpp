/*
 * This file is part of file_ingest_key_analyser (FIKA).
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

#include <sstream>

#include "cli_parser.h"

using namespace fika;

TEST_CASE("cli_parser basic parsing") {
  const char* argv[] = {"program", "--config", "my.cfg", "--verbose"};
  int argc = 4;

  bool callback_called = false;
  std::string config_value;

  cli_parser cli(argc, const_cast<char**>(argv));

  cli.add_option<bool>("verbose", "Verbose mode",
                       [&](bool val) { callback_called = val; });

  cli.add_option<std::string>(
      "config", "Config file",
      [&](const std::string& val) { config_value = val; }, "default.cfg");

  auto vm = cli.parse();

  // Check variables_map values
  CHECK(vm->count("verbose") == 1);
  CHECK(vm.value()["verbose"].as<bool>() == true);
  CHECK(vm->count("config") == 1);
  CHECK(vm.value()["config"].as<std::string>() == "my.cfg");

  // Check callbacks triggered
  CHECK(callback_called == true);
  CHECK(config_value == "my.cfg");
}

TEST_CASE("cli_parser default values") {
  const char* argv[] = {"program"};
  int argc = 1;

  std::string config_value;
  bool verbose_flag = false;

  cli_parser cli(argc, const_cast<char**>(argv));

  cli.add_option<std::string>(
      "config", "Config file",
      [&](const std::string& val) { config_value = val; }, "default.cfg");

  cli.add_option<bool>("verbose", "Verbose mode",
                       [&](bool val) { verbose_flag = val; });

  auto vm = cli.parse();

  // Check defaults
  CHECK(vm->count("config") == 1);
  CHECK(vm.value()["config"].as<std::string>() == "default.cfg");

  // Boolean flag not passed but present since it's a switch
  CHECK(vm->count("verbose") == 1);

  // Callbacks
  CHECK(config_value == "default.cfg");
  CHECK(verbose_flag == false);
}

TEST_CASE("cli_parser multiple callbacks") {
  const char* argv[] = {"program", "--foo", "bar", "--baz"};
  int argc = 4;

  std::string foo_val;
  bool baz_called = false;

  cli_parser cli(argc, const_cast<char**>(argv));

  cli.add_option<std::string>("foo", "Foo value",
                              [&](const std::string& val) { foo_val = val; });

  cli.add_option<bool>("baz", "Baz flag", [&](bool val) { baz_called = val; });

  auto vm = cli.parse();

  CHECK(foo_val == "bar");
  CHECK(baz_called == true);
}

TEST_CASE("cli_parser bool option") {
  const char* argv[] = {"program", "--verbose"};
  int argc = 2;

  bool verbose_value = false;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<bool>("verbose", "Enable verbose",
                       [&](bool val) { verbose_value = val; });

  auto vm = cli.parse();

  // Check variables_map
  CHECK(vm.value()["verbose"].as<bool>() == true);
  // Check callback was triggered correctly
  CHECK(verbose_value == true);
}

TEST_CASE("cli_parser string option") {
  const char* argv[] = {"program", "--config", "my_config.cfg"};
  int argc = 3;

  std::string config_value;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<std::string>(
      "config", "Config file",
      [&](const std::string& val) { config_value = val; }, "default.cfg");

  auto vm = cli.parse();

  CHECK(vm.value()["config"].as<std::string>() == "my_config.cfg");
  CHECK(config_value == "my_config.cfg");
}

TEST_CASE("cli_parser int option") {
  const char* argv[] = {"program", "--threads", "8"};
  int argc = 3;

  int threads_value = 0;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<int>(
      "threads", "Number of threads", [&](int val) { threads_value = val; }, 4);

  auto vm = cli.parse();

  CHECK(vm.value()["threads"].as<int>() == 8);
  CHECK(threads_value == 8);
}

TEST_CASE("cli_parser default values") {
  const char* argv[] = {"program"};
  int argc = 1;

  bool verbose_value = false;
  std::string config_value;
  int threads_value = 0;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<bool>("verbose", "Enable verbose",
                       [&](bool val) { verbose_value = val; });

  cli.add_option<std::string>(
      "config", "Config file",
      [&](const std::string& val) { config_value = val; }, "default.cfg");

  cli.add_option<int>(
      "threads", "Number of threads", [&](int val) { threads_value = val; }, 4);

  auto vm = cli.parse();

  // Bool default
  CHECK(vm.value()["verbose"].as<bool>() == false);
  CHECK(verbose_value == false);

  // String default
  CHECK(vm.value()["config"].as<std::string>() == "default.cfg");
  CHECK(config_value == "default.cfg");

  // Int default
  CHECK(vm.value()["threads"].as<int>() == 4);
  CHECK(threads_value == 4);
}

TEST_CASE("cli_parser print_help") {
  const char* argv[] = {"program"};
  int argc = 1;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<std::string>("config", "Config file", nullptr, "default.cfg");

  std::ostringstream oss;
  auto* old_cout = std::cout.rdbuf(oss.rdbuf());  // redirect cout

  cli.print_help();

  std::cout.rdbuf(old_cout);  // restore cout

  std::string output = oss.str();
  CHECK(output.find("config") != std::string::npos);  // help includes "config"
}

TEST_CASE("cli_parser triggers boost::program_options::error") {
  const char* argv[] = {"program", "--threads", "notanumber"};
  int argc = 3;

  cli_parser cli(argc, const_cast<char**>(argv));
  cli.add_option<int>("threads", "Number of threads");

  std::ostringstream oss;
  auto* old_cout = std::cout.rdbuf(oss.rdbuf());  // redirect cout

  cli.parse();

  std::cout.rdbuf(old_cout);  // restore cout

  std::string output = oss.str();
  CHECK(output.find("threads") != std::string::npos);  // help includes "config"
}

TEST_CASE("cli_parser init sets predefined options") {
  // Simulate CLI args
  const char* argv[] = {
      "fika_service",             // binary name
      "--pidfile=/tmp/test.pid",  // set pidfile
      "--verbose",                // set verbose flag
  };
  int argc = sizeof(argv) / sizeof(argv[0]);

  cli_parser parser(argc, const_cast<char**>(argv));
  parser.initialize(fika::default_option::verbose |
                    fika::default_option::pidfile);
  parser.parse();

  SUBCASE("get_option returns correct pidfile path") {
    auto pidfile = parser.get_option<std::string>("pidfile");
    CHECK(pidfile == "/tmp/test.pid");
  }

  SUBCASE("get_option returns true for verbose") {
    auto verbose = parser.get_option<bool>("verbose");
    CHECK(verbose == true);
  }
}

TEST_CASE("cli_parser uses default values when not specified") {
  const char* argv[] = {
      "fika_service", "--pidfile=/tmp/test.pid",  // set pidfile
  };
  int argc = sizeof(argv) / sizeof(argv[0]);

  cli_parser parser(argc, const_cast<char**>(argv));
  parser.initialize(fika::default_option::verbose |
                    fika::default_option::pidfile);
  parser.parse();

  auto verbose = parser.get_option<bool>("verbose");
  CHECK(verbose == false);
}

TEST_CASE("cli_parser get_option throws on missing key") {
  const char* argv[] = {"fika_service"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  cli_parser parser(argc, const_cast<char**>(argv));
  parser.parse();  // no init

  CHECK_THROWS_AS(parser.get_option<std::string>("nonexistent"),
                  std::runtime_error);
}