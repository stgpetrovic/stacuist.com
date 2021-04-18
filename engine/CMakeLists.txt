add_library(engine recipe.h model.h model.cc)

target_link_libraries(engine
  wtdbo
)
