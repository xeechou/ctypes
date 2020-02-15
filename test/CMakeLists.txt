include(CTest)

include_directories(${CTYPES_INCLUDE_DIR})

set(TEST_SRCS
  test_smallfuncs.c
  test_vtree.c
  test_vector.c
  test_hash.c
  )


foreach(src ${TEST_SRCS})
  get_filename_component(tgt ${src} NAME_WE)
  add_executable(${tgt}
    ${src})
  target_link_libraries(${tgt}
    ctypes)
  add_test(${tgt} ${tgt})
  list(APPEND CTYPES_TESTS ${tgt})
endforeach()

add_custom_target(check_ctypes
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS ${CTYPES_TESTS})
