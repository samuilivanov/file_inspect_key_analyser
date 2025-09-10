cmake_minimum_required(VERSION 3.15...4.0)

project(
  fika
  VERSION 0.1
  DESCRIPTION "A toolkit that detects and extracts metadata and text"
  LANGUAGES CXX)

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
endif()
