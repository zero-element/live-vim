project(server C)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../bin)

add_executable(server main.c)

target_link_libraries(${PROJECT_NAME} common_static)