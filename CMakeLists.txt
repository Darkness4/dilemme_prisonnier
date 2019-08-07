cmake_minimum_required(VERSION 3.0.0)
project(dilemme_prisonnier VERSION 1.1.0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_library(ERROR_HANDLER error_handler HINTS lib)
find_library(LIGNE ligne HINTS lib)
find_library(RESOLV resolv HINTS lib)

message("---Listing local depedencies---")
message("${ERROR_HANDLER}")
message("${LIGNE}")
message("${RESOLV}")
message("---End listing local depedencies---")

include_directories(include)

add_subdirectory(src/client)
add_subdirectory(src/server)

execute_process(
    COMMAND /bin/sh ./scripts/generate-config.sh
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
