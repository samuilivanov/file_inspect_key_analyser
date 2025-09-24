# Testing library
enable_testing()

function(add_doctest name)
  set(options) # unused
  set(oneValueArgs)
  set(multiValueArgs SRCS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${name} ${ARG_SRCS})
  target_compile_features(${name} PRIVATE cxx_std_23)

  # Include paths
  target_include_directories(${name} PRIVATE ../include)

  # Libraries (common for all tests)
  target_link_libraries(${name} PRIVATE global util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)

  # Register with CTest
  add_test(NAME ${name} COMMAND ${name})
endfunction()
add_doctest(test_logger SRCS test_logger.cpp)
add_doctest(test_qn_rslv SRCS test_qn_rslv.cpp)
add_doctest(test_pickup SRCS test_pickup.cpp)
