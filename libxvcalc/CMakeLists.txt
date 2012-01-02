cmake_minimum_required (VERSION 2.6)
OPTION(BUILD_SHARED_LIBS "Build shared library." ON)
project (XVCalc)

add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tokenize.h ${CMAKE_CURRENT_SOURCE_DIR}/tokenize.c
  COMMAND flex --nounistd --prefix=xvcalc_yy --header-file=${CMAKE_CURRENT_SOURCE_DIR}/tokenize.h --outfile=${CMAKE_CURRENT_SOURCE_DIR}/tokenize.c ${CMAKE_CURRENT_SOURCE_DIR}/tokenize.l
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tokenize.l
  )
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.h ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
  COMMAND bison --name-prefix=xvcalc_yy --defines=${CMAKE_CURRENT_SOURCE_DIR}/parser.h --output=${CMAKE_CURRENT_SOURCE_DIR}/parser.c ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
)

add_library(xvcalc
	xvcalc.c
	XvcOperators.c
	tree.c cleanup.c functions.c parser.c tokenize.c)
add_executable(xvctest xvctest.c)
target_link_libraries(xvctest xvcalc)

install (TARGETS xvcalc DESTINATION lib)
install (FILES xvcalc.h DESTINATION include)
