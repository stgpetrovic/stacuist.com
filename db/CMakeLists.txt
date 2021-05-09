file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources)

execute_process(
  COMMAND python3 init_db.py ${CMAKE_CURRENT_BINARY_DIR}/resources/recipes.db
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
