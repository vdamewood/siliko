INCLUDE_DIRECTORIES(${XVCALC_SOURCE_DIR}/libxvcalc)

add_executable(xvctest xvctest.c)
target_link_libraries(xvctest xvcalc)
