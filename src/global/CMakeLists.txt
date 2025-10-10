file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${fika_SOURCE_DIR}/global/*.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(global src/message_queues.cpp src/icp_client.cpp src/service.cpp src/file_job.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(global PUBLIC include .)

# This depends on (header only) boost
target_link_libraries(global PRIVATE util Boost::boost Boost::log
  Boost::thread
  Boost::system)

# All users of this library will need at least C++11
target_compile_features(global PUBLIC cxx_std_23)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

install(TARGETS global
  LIBRARY DESTINATION ${FIKA_LIBDIR}
  ARCHIVE DESTINATION ${FIKA_LIBDIR}
)
