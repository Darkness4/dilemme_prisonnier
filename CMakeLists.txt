cmake_minimum_required(VERSION 3.0.0)
project(dilemme_prisonnier VERSION 1.1.0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_library(
    error_handler SHARED
    shared/error_handler/error_handler.c
)
add_library(
    ligne SHARED
    shared/ligne/ligne.c
)
add_library(
    resolv SHARED
    shared/resolv/resolv.c
)

include_directories(
    shared/error_handler
    shared/ligne
    shared/resolv
)

add_subdirectory(src/client)
add_subdirectory(src/server)

execute_process(
    COMMAND /bin/sh ./scripts/generate-config.sh
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})