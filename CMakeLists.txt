cmake_minimum_required(VERSION 3.10)
project(cheap C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Werror)

add_library(heap STATIC heap.c)
target_include_directories(heap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  add_executable(test_function test/test_function.c)
  target_link_libraries(test_function PRIVATE heap)

  add_executable(test_perf test/test_perf.c)
  target_link_libraries(test_perf PRIVATE heap)
endif()

set_target_properties(heap PROPERTIES OUTPUT_NAME heap)
