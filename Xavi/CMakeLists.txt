# Xavi

set(Xavi_BISON_INPUT  ${CMAKE_CURRENT_SOURCE_DIR}/XaviYyParser.y)
set(Xavi_BISON_HEADER ${CMAKE_CURRENT_BINARY_DIR}/XaviYyParser.h)
set(Xavi_BISON_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/XaviYyParser.c)
set(Xavi_BISON_OUTPUT ${Xavi_BISON_HEADER} ${Xavi_BISON_SOURCE})

set(Xavi_FLEX_INPUT  ${CMAKE_CURRENT_SOURCE_DIR}/XaviYyLexer.l)
set(Xavi_FLEX_HEADER ${CMAKE_CURRENT_BINARY_DIR}/XaviYyLexer.h)
set(Xavi_FLEX_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/XaviYyLexer.c)
set(Xavi_FLEX_OUTPUT ${Xavi_FLEX_HEADER} ${Xavi_FLEX_SOURCE})

set(Xavi_PUBLIC_HEADERS Xavi.h)
set(Xavi_INTERNAL_HEADERS
  XaviStructs.h
  XaviFunctionCall.h XaviOperatorCall.h
  XaviArglist.h XaviFunctionId.h XaviTree.h
  XaviEvaluate.h XaviCleanup.h
  ${Xavi_BISON_HEADER}
  ${Xavi_FLEX_HEADER})
set(Xavi_SOURCES
  Xavi.c
  XaviFunctionCall.c XaviOperatorCall.c
  XaviArglist.c XaviFunctionId.c XaviTree.c
  XaviEvaluate.c XaviCleanup.c
  ${Xavi_BISON_SOURCE}
  ${Xavi_FLEX_SOURCE})

set(Xavi_YY_PREFIX Xavi_yy)

set(Xavi_BISON_ARGS
  --name-prefix=${Xavi_YY_PREFIX}
  --defines=${Xavi_BISON_HEADER}
  --output=${Xavi_BISON_SOURCE}
  ${Xavi_BISON_INPUT})
set(Xavi_FLEX_ARGS
  --nounistd
  --prefix=${Xavi_YY_PREFIX}
  --header-file=${Xavi_FLEX_HEADER}
  --outfile=${Xavi_FLEX_SOURCE}
  ${Xavi_FLEX_INPUT})

add_custom_command(
  OUTPUT  ${Xavi_BISON_OUTPUT}
  COMMAND ${BISON_EXECUTABLE} ${Xavi_BISON_ARGS}
  DEPENDS ${Xavi_BISON_INPUT})

add_custom_command(
  OUTPUT  ${Xavi_FLEX_OUTPUT}
  COMMAND ${FLEX_EXECUTABLE} ${Xavi_FLEX_ARGS}
  DEPENDS ${Xavi_FLEX_INPUT})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

add_library(Xavi
  ${Xavi_PUBLIC_HEADERS}
  ${Xavi_INTERNAL_HEADERS}
  ${Xavi_SOURCES})
target_link_libraries(Xavi
  ${MATH_LIB})
set_target_properties(Xavi
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER ${Xavi_PUBLIC_HEADERS})

install(TARGETS Xavi
  FRAMEWORK DESTINATION . COMPONENT Runtime)
