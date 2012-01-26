# XVCalc

set(XVCalc_BISON_INPUT  ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyParser.y)
set(XVCalc_BISON_HEADER ${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.h)
set(XVCalc_BISON_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/XvcYyParser.c)
set(XVCalc_BISON_OUTPUT ${XVCalc_BISON_HEADER} ${XVCalc_BISON_SOURCE})

set(XVCalc_FLEX_INPUT  ${CMAKE_CURRENT_SOURCE_DIR}/XvcYyLexer.l)
set(XVCalc_FLEX_HEADER ${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.h)
set(XVCalc_FLEX_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/XvcYyLexer.c)
set(XVCalc_FLEX_OUTPUT ${XVCalc_FLEX_HEADER} ${XVCalc_FLEX_SOURCE})

set(XVCalc_PUBLIC_HEADERS XVCalc.h)
set(XVCalc_INTERNAL_HEADERS
  XvcStructs.h
  XvcState.h
  XvcFunctionCall.h XvcOperatorCall.h
  XvcArglist.h XvcFunctionId.h XvcTree.h
  XvcEvaluate.h XvcCleanup.h
  ${XVCalc_BISON_HEADER}
  ${XVCalc_FLEX_HEADER})
set(XVCalc_SOURCES
  XVCalc.c XvcState.c
  XvcFunctionCall.c XvcOperatorCall.c
  XvcArglist.c XvcFunctionId.c XvcTree.c
  XvcEvaluate.c XvcCleanup.c
  ${XVCalc_BISON_SOURCE}
  ${XVCalc_FLEX_SOURCE})

set(XVCalc_YY_PREFIX Xvc_yy)

set(XVCalc_BISON_ARGS
  --name-prefix=${XVCalc_YY_PREFIX}
  --defines=${XVCalc_BISON_HEADER}
  --output=${XVCalc_BISON_SOURCE}
  ${XVCalc_BISON_INPUT})
set(XVCalc_FLEX_ARGS
  --nounistd
  --prefix=${XVCalc_YY_PREFIX}
  --header-file=${XVCalc_FLEX_HEADER}
  --outfile=${XVCalc_FLEX_SOURCE}
  ${XVCalc_FLEX_INPUT})

add_custom_command(
  OUTPUT  ${XVCalc_BISON_OUTPUT}
  COMMAND ${BISON_EXECUTABLE} ${XVCalc_BISON_ARGS}
  DEPENDS ${XVCalc_BISON_INPUT})

add_custom_command(
  OUTPUT  ${XVCalc_FLEX_OUTPUT}
  COMMAND ${FLEX_EXECUTABLE} ${XVCalc_FLEX_ARGS}
  DEPENDS ${XVCalc_FLEX_INPUT})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

add_library(XVCalc
  ${XVCalc_PUBLIC_HEADERS}
  ${XVCalc_INTERNAL_HEADERS}
  ${XVCalc_SOURCES})
target_link_libraries(XVCalc
  ${MATH_LIB})
set_target_properties(XVCalc
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER ${XVCalc_PUBLIC_HEADERS})
