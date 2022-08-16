add_executable( create_test create_test.cpp )
target_include_directories(create_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(create_test airbrushlib)
add_test( NAME create COMMAND create_test )

add_executable( paint_test paint_test.cpp )
target_include_directories(paint_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(paint_test airbrushlib)
add_test( NAME paint COMMAND paint_test )
