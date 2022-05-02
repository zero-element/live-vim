project(client C)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../bin)

file(GLOB SOURCES *.c *.h)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Curses REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} common_static)