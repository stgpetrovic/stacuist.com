find_package(Boost COMPONENTS system thread)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(libs ${libs}    ${Boost_LIBRARIES})
endif()

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

add_executable(${target} ${src})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_link_libraries(${target}
  ${libs}

  absl::strings

  wt
  wtfcgi
  wtdbo
  wtdbosqlite3
)
else()
  target_link_libraries(${target}
  ${libs}

  absl::strings

  wt
  wthttp
  wtdbo
  wtdbosqlite3
)
endif()
