file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/detect/include/*.h")


add_executable(detect src/main.cpp src/magic_handle.cpp src/detector.cpp ${HEADER_LIST})
target_include_directories(detect PUBLIC include)
target_compile_features(detect PRIVATE cxx_std_23)

target_link_libraries(detect PRIVATE global magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
