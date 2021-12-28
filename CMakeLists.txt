cmake_minimum_required (VERSION 3.16.0)
project (toy)

message(STATUS "Boost_DIR=$ENV{Boost_DIR}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.77 REQUIRED COMPONENTS coroutine)

set(CMAKE_CXX_STANDARD 20)

include_directories (
  SYSTEM ${Boost_INCLUDE_DIRS}
  SYSTEM submodules/fmt/include
  submodules/urdl/include
  .
  ${CMAKE_SOURCE_DIR}/src/
)

add_compile_options(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING=1)
add_compile_options(-DBOOST_BEAST_USE_STD_STRING_VIEW)
add_compile_options(-DURDL_HEADER_ONLY)
add_compile_options(-Wall)

add_subdirectory(submodules)
add_subdirectory(src/cpp20)
add_subdirectory(src/cpp17)
