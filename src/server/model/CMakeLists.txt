set(
  model_source_files
  client_thread.c
  client_thread.h
  configuration.c
  configuration.h
  datacontext.c
  datacontext.h
  joueur.c
  joueur.h
  match.c
  match.h
)
add_library(model ${model_source_files})

target_link_libraries(
  model
  "${ERROR_HANDLER}"
  "${LIGNE}"
  view
  Threads::Threads
)