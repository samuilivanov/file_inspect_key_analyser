cmake_minimum_required(VERSION 3.15...4.0)

project(
  fika
  VERSION 0.1
  DESCRIPTION "A toolkit that detects and extracts metadata and text"
  LANGUAGES CXX)
  
  option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
  if(ENABLE_COVERAGE)
      message(STATUS "Building with coverage flags")
      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
          add_compile_options(--coverage -O0 -g)
          add_link_options(--coverage)
      endif()
  endif()

  if(ENABLE_COVERAGE)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    if(LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND ${LCOV} --directory . --zerocounters
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info '/usr/*' --output-file coverage.info
            COMMAND ${GENHTML} coverage.info --output-directory coverage-report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests and generating coverage report"
        )
    else()
        message(WARNING "lcov or genhtml not found! Coverage target will not work.")
    endif()
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  set(CMAKE_CXX_EXTENSIONS OFF)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)

  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(httplib)

find_package(Boost REQUIRED COMPONENTS log thread system)

# Install locations (can be overridden via -D option)
set(FIKA_BINDIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Directory for binaries")
set(FIKA_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Directory for libraries")
set(FIKA_DATADIR "${CMAKE_BINARY_DIR}/conf" CACHE PATH "Directory for config/data files" FORCE)
set(FIKA_SPOOL_DIR "${CMAKE_BINARY_DIR}/spool/fika")
file(MAKE_DIRECTORY ${FIKA_SPOOL_DIR})

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
)
include_directories(${CMAKE_BINARY_DIR})

# Create local conf folder in build folder
set(FIKA_CONF_DIR "${CMAKE_BINARY_DIR}/conf")
file(MAKE_DIRECTORY ${FIKA_CONF_DIR})

# Generate JSON config from template
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/supervisor.json.in
    ${FIKA_CONF_DIR}/supervisor.json
    @ONLY
)
# Where executables go after build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Where shared libraries go after build
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Where static libraries go
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src/fika)
add_subdirectory(src/global) 
add_subdirectory(src/util)
add_subdirectory(src/detect)
add_subdirectory(src/qm)
add_subdirectory(src/ingestd)
add_subdirectory(src/supervisor)
add_subdirectory(src/parse)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(src/tests)
  add_subdirectory(src/util/tests)
  add_subdirectory(src/global/tests)
  add_subdirectory(src/supervisor/tests)
  add_subdirectory(src/detect/tests)
endif()
