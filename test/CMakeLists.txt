add_executable( create_test create_test.cpp )
target_include_directories(create_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(create_test airbrushlib)
add_test( NAME create COMMAND create_test )
