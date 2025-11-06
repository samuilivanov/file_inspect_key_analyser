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
add_doctest(test_cmd SRCS test_cmd.cpp ../src/supervisor.cpp ../src/worker.cpp)
# add_doctest(test_supervisor SRCS test_supervisor.cpp ../src/worker.cpp ../src/supervisor.cpp)
# add_doctest(test_worker SRCS test_worker.cpp ../src/worker.cpp)
