file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/qm/include/*.h")


add_executable(qm src/main.cpp src/qm.cpp src/qipc.cpp src/state_machine.cpp ${HEADER_LIST})
target_include_directories(qm PUBLIC include ../global)
target_compile_features(qm PRIVATE cxx_std_23)

target_link_libraries(qm PRIVATE magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
