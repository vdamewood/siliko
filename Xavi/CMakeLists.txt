# Xavi

if (UNIX)
	add_definitions(-DXAVI_USE_UNIX=1)
else()
	add_definitions(-DXAVI_USE_UNIX=0)
endif()

set(Xavi_PUBLIC_HEADERS
  Value.h
  FunctionCaller.h
  SyntaxTree.h
  DataSource.h
  StringSource.h
  Lexer.h
  InfixParser.h
)

set(Xavi_HEADERS
  ${Xavi_PUBLIC_HEADERS})

set(Xavi_SOURCES
  FunctionCaller.c
  SyntaxTree.c
  DataSource.c
  StringSource.c
  Lexer.c
  InfixParser.c
)

add_library(Xavi
  ${Xavi_HEADERS}
  ${Xavi_SOURCES})
target_link_libraries(Xavi
  ${MATH_LIB})
set_target_properties(Xavi
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER ${Xavi_PUBLIC_HEADERS})

install(TARGETS Xavi
  RUNTIME       DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE       DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY       DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK     DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}/Xavi
)
