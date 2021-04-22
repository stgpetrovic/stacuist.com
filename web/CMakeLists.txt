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

target_link_libraries(tags
  wtdbo
)

add_executable(${target} ${src})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_link_libraries(${target}
  ${libs}

  absl::strings
  absl::random_random
  absl::algorithm

  glog::glog

  wt
  wtfcgi
  wtdbo
  wtdbosqlite3

  tags
)
else()
  target_link_libraries(${target}
  ${libs}

  absl::strings
  absl::random_random
  absl::algorithm

  glog::glog

  wt
  wthttp
  wtdbo
  wtdbosqlite3

  tags
)
endif()
