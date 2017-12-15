cmake_minimum_required(VERSION 3.1.0)
project(ctypes
  VERSION 0.1
  LANGUAGES C )

add_definitions(-D_GNU_SOURCE)
if (NOT CMAKE_BINARY_DIR)
  set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
endif()
#set(CMAKE_C_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TYPES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

#include_directories(include)
add_subdirectory(src)
