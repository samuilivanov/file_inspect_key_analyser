file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/src/parsers/pdf/include/*.h")

add_executable(pdf src/main.cpp src/pdf_parser.cpp ${HEADER_LIST})
target_include_directories(pdf PUBLIC include ../global/include)
target_compile_features(pdf PRIVATE cxx_std_23)

target_link_libraries(pdf PRIVATE global magic util fmt poppler-cpp Boost::boost Boost::log
    Boost::thread
    Boost::system)

install(TARGETS pdf
    RUNTIME DESTINATION ${FIKA_BINDIR}
)