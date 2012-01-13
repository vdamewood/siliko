cmake_minimum_required (VERSION 2.6)
OPTION(BUILD_SHARED_LIBS "Build shared library." ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(XvcBisonArgs "--name-prefix=Xvc_yy --defines=${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.h")
set(XvcFlexArgs "--nounistd --prefix=Xvc_yy --header-file=${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.h")

separate_arguments(XvcBisonArgs)
separate_arguments(XvcFlexArgs)
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.h ${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.c
  COMMAND bison ${XvcBisonArgs} --output=${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.c ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyParser.y
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyParser.y
)
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.h ${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.c
  COMMAND flex ${XvcFlexArgs} --outfile=${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.c ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyLexer.l
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyLexer.l
)

add_library(xvcalc
	XVCalc.c
	XvcState.c
	XvcFunctionCall.c XvcOperatorCall.c
	XvcArglist.c XvcFunctionId.c XvcTree.c
	XvcEvaluate.c
	XvcCleanup.c
	XvcYyParser.c XvcYyLexer.c
)
target_link_libraries(xvcalc ${MATH_LIB})

install (TARGETS xvcalc DESTINATION lib)
install (FILES XVCalc.h DESTINATION include)
