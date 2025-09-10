file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/parse/include/*.h")

add_executable(parse src/main.cpp 
    src/parser_service.cpp 
    src/parser_registry.cpp 
    src/parser_worker.cpp ${HEADER_LIST})
target_include_directories(parse PUBLIC include ../global ${httplib_SOURCE_DIR})
target_compile_features(parse PRIVATE cxx_std_23)

target_link_libraries(parse PRIVATE global magic util fmt Boost::boost Boost::log
    Boost::thread
    Boost::system)
