# Testing library
enable_testing()

function(add_doctest name lib)
  add_executable(${name} ${name}.cpp ../src/worker.cpp ../src/supervisor.cpp)
  target_compile_features(${name} PRIVATE cxx_std_23)
  target_include_directories(${name} PRIVATE ../include)
  target_link_libraries(${name} PRIVATE global util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
endfunction()
add_doctest(test_worker supervisor)
add_doctest(test_supervisor supervisor)
add_doctest(test_cmd supervisor)
# add_doctest(test_loggercopy util)
add_test(NAME test_worker COMMAND test_worker)
add_test(NAME test_supervisor COMMAND test_supervisor)
add_test(NAME test_cmd COMMAND test_cmd)
# add_test(NAME test_loggercopy COMMAND test_loggercopy)
