file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/pickup/include/*.h")

add_executable(pickup src/main.cpp
    ${HEADER_LIST})
target_include_directories(pickup PUBLIC include ../global/include ${httplib_SOURCE_DIR})
target_compile_features(pickup PRIVATE cxx_std_23)

target_link_libraries(pickup PRIVATE global magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)

install(TARGETS pickup
    RUNTIME DESTINATION ${FIKA_BINDIR}
)
