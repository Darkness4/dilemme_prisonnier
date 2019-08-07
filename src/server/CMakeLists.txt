cmake_minimum_required(VERSION 3.0.0)
project(dilemme_prisonnier_server)
set(C_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(deroulement)
add_subdirectory(model)
add_subdirectory(view)

set(server_source_files server.c)
add_executable(dilemme_prisonnier_server ${server_source_files})

target_link_libraries(
    dilemme_prisonnier_server
    deroulement
    error_handler
    ligne
    resolv
    model
    Threads::Threads
)