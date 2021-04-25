cmake_minimum_required(VERSION 2.8)
project (stacuist.com CXX)

set(CMAKE_CXX_STANDARD   14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc++17-extensions")

set(ABSL_CXX_STANDARD "${CMAKE_CXX_STANDARD}")

# Import Abseil's CMake targets
find_package(absl REQUIRED)

###########
# Debug or Release
###########
if (NOT CMAKE_BUILD_TYPE)
  ## set default to Debug
  set(CMAKE_BUILD_TYPE Debug)  # override with -DCMAKE_BUILD_TYPE=Debug
  message("==> CMAKE_BUILD_TYPE empty. Changing it to Debug.")
else()
  message("==> CMAKE_BUILD_TYPE == ${CMAKE_BUILD_TYPE}.")
endif()

add_subdirectory(engine)
add_subdirectory(web)
