file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/ingestd/include/*.h")


add_executable(ingestd src/main.cpp src/ingestd.cpp ${HEADER_LIST})
target_include_directories(ingestd PUBLIC include ../global/include ${httplib_SOURCE_DIR})
target_compile_features(ingestd PRIVATE cxx_std_23)

target_link_libraries(ingestd PRIVATE magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
install(TARGETS ingestd
        RUNTIME DESTINATION ${FIKA_BINDIR}
)