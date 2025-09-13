file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/fika/include/*.h")


add_executable(fika src/main.cpp src/cli.cpp ${HEADER_LIST})
target_include_directories(fika PUBLIC include ../global ../global/include ${httplib_SOURCE_DIR})
target_compile_features(fika PRIVATE cxx_std_23)

target_link_libraries(fika PRIVATE magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system
    Boost::program_options)

install(TARGETS fika
    RUNTIME DESTINATION ${FIKA_BINDIR}
)