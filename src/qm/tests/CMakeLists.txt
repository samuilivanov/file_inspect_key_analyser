# Testing library
enable_testing()

function(add_doctest name lib)
  add_executable(${name} ${name}.cpp)
  target_compile_features(${name} PRIVATE cxx_std_23)
  target_include_directories(${name} PRIVATE ../include)
  target_link_libraries(${name} PRIVATE global util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
endfunction()
add_doctest(test_qipc global)
# add_doctest(test_loggercopy util)
add_test(NAME test_qipc COMMAND test_qipc)
# add_test(NAME test_loggercopy COMMAND test_loggercopy)
