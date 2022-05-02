project(common C)

add_library(${PROJECT_NAME}_static STATIC
    dstring.c
    utils.c
    networking.c
    thread.c
)

target_include_directories(${PROJECT_NAME}_static PUBLIC ${PROJECT_SOURCE_DIR})