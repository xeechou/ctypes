cmake_minimum_required(VERSION 3.1.0)
project(ctypes
  VERSION 0.1
  LANGUAGES C)


include(CTest)

add_definitions(-D_GNU_SOURCE)
if (NOT CMAKE_BINARY_DIR)
  set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ctype STATIC
  src/list.c
  src/vector.c
  src/vtree.c
  src/stack.c
  src/dhash.c
  src/queue.c

  src/os/buffer.c
  src/os/os-compatibility.c
  src/os/exec.c
  src/os/file.c)

target_include_directories(ctype
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#keep the old names for compatibilities
find_path(CTYPES_INCLUDE_DIR helpers.h
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CTYPES_LIBRARIES ctype CACHE STRING "ctypes library")

add_subdirectory(test)
