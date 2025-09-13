# Testing library
enable_testing()

function(add_doctest name lib)
  add_executable(${name} ${name}.cpp ../src/detector.cpp ../src/magic_handle.cpp)
  target_compile_features(${name} PRIVATE cxx_std_23)
  target_include_directories(${name} PRIVATE ../include ../include/detectors)
  target_link_libraries(${name} PRIVATE magic global util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
endfunction()

add_doctest(test_magic_detector detect)
add_doctest(test_detector detect)
# add_doctest(test_loggercopy util)
add_test(NAME test_magic_detector COMMAND test_magic_detector)
add_test(NAME test_detector COMMAND test_detector)
# add_test(NAME test_loggercopy COMMAND test_loggercopy)
