execute_process(
  COMMAND python3 init_db.py /tmp/recipes.db
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
