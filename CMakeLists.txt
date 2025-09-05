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

find_package(Boost REQUIRED)

add_subdirectory(src/fika)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
# if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#    AND BUILD_TESTING)
#   add_subdirectory(tests)
# endif()
