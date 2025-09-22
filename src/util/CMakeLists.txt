file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/util/*.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(util src/msg.cpp src/qn_rslv.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(util PUBLIC include)

# This depends on (header only) boost
target_link_libraries(util PRIVATE Boost::boost Boost::log
    Boost::thread
    Boost::system)

# All users of this library will need at least C++11
target_compile_features(util PUBLIC cxx_std_23)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

install(TARGETS util
        LIBRARY DESTINATION ${FIKA_LIBDIR}
        ARCHIVE DESTINATION ${FIKA_LIBDIR}
)