find_package(Boost COMPONENTS system thread)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(libs ${libs}    ${Boost_LIBRARIES})
endif()

find_package (glog 0.5.0 REQUIRED)

##  FindWt.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE( Wt REQUIRED )

INCLUDE_DIRECTORIES( ${Wt_INCLUDE_DIR} )
INCLUDE_DIRECTORIES(/usr/local/include/Wt)
INCLUDE_DIRECTORIES(../)

set(target stacuist.com)

set(src
  app.cc
)

add_library(tags tags.h tags.cc)
add_library(recipe_view recipe_view.h recipe_view.cc)
add_library(query query.h query.cc)

target_link_libraries(tags
  wtdbo
)

add_executable(${target} ${src})

# Deploy on fastcgi in release, and local httpd in debug mode.
set (wt_deploy_lib
  debug wthttp
  optimized wtfcgi)

target_link_libraries(${target}
  ${libs}

  # External libraries.
  absl::strings
  absl::random_random
  absl::algorithm
  absl::flags
  absl::flags_parse
  absl::statusor
  glog::glog

  # Wt libraries.
  wt
  wtdbo
  wtdbosqlite3
  ${wt_deploy_lib}

  # Internal libraries.
  tags
  recipe_view
  query
)
