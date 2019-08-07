set(
  view_source_files
  joueur_view.c
  joueur_view.h
  match_view.c
  match_view.h
)
add_library(view ${view_source_files})

target_link_libraries(
  view 
  "${ERROR_HANDLER}"
  "${LIGNE}"
  model
)