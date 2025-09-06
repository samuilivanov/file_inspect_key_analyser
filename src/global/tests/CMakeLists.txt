# Testing library
enable_testing()

function(add_doctest name lib)
  add_executable(${name} ${name}.cpp)
  target_compile_features(${name} PRIVATE cxx_std_23)
  target_link_libraries(${name} PRIVATE util fmt Boost::boost Boost::log)
endfunction()

add_doctest(test_config_loader util)
# add_doctest(test_loggercopy util)
add_test(NAME test_config_loader COMMAND test_config_loader)
# add_test(NAME test_loggercopy COMMAND test_loggercopy)
