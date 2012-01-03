INCLUDE_DIRECTORIES(${XVCALC_SOURCE_DIR}/libxvcalc)

add_executable(XvcBasicTest XvcBasicTest.c)
target_link_libraries(XvcBasicTest xvcalc)
