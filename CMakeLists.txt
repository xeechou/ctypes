cmake_minimum_required(VERSION 3.1.0)
project(ctypes
  VERSION 0.1
  LANGUAGES C )

add_definitions(-D_GNU_SOURCE)
if (NOT CMAKE_BINARY_DIR)
  set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CTYPES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE PATH "ctypes include directories")

add_library(ctype STATIC
  src/list.c
  src/vector.c
  src/vtree.c
  src/buffer.c
  src/stack.c
  src/dhash.c
  )

set(CTYPES_LIBRARIES ctype CACHE STRING "ctypes library")
