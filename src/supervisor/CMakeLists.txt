file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/supervisor/include/*.h")


add_executable(supervisor src/main.cpp src/supervisor.cpp src/worker.cpp ${HEADER_LIST})
target_include_directories(supervisor PUBLIC include ../global ../global/include ${httplib_SOURCE_DIR})
target_compile_features(supervisor PRIVATE cxx_std_23)

target_link_libraries(supervisor PRIVATE magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)

install(TARGETS supervisor
    RUNTIME DESTINATION ${FIKA_BINDIR}
)