cmake_minimum_required(VERSION 3.0.0)
project(dilemme_prisonnier_client)
set(C_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)

set(client_source_files client.c)
add_executable(dilemme_prisonnier_client ${client_source_files})

target_link_libraries(
    dilemme_prisonnier_client
    error_handler
    ligne
    resolv
    pthread
    Threads::Threads
)
