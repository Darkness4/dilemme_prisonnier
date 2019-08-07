set(
  deroulement_source_files
  deroulement.c
  deroulement.h
  match_thread.c
  match_thread.h
)
add_library(deroulement ${deroulement_source_files})

target_link_libraries(
  deroulement
  error_handler
  model
  view
  ligne
  Threads::Threads
)