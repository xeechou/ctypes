cmake_minimum_required(VERSION 3.1.0)
project(ctypes
  VERSION 0.2
  LANGUAGES C)


add_definitions(-D_GNU_SOURCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ctypes STATIC
  src/list.c
  src/vector.c
  src/vtree.c
  src/stack.c
  src/dhash.c
  src/queue.c
  src/radix/radix.c

  src/os/buffer.c
  src/os/os-compatibility.c
  src/os/exec.c
  src/os/file.c)

target_include_directories(ctypes
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ctypes PUBLIC m)

#keep the old names for compatibilities
find_path(CTYPES_INCLUDE_DIR "ctypes/helpers.h"
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CTYPES_LIBRARIES ctypes CACHE STRING "ctypes library")

add_subdirectory(test)
