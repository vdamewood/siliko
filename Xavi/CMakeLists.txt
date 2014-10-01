# Xavi

if (UNIX)
	add_definitions(-DXAVI_USE_UNIX=1)
else()
	add_definitions(-DXAVI_USE_UNIX=0)
endif()

set(Xavi_HEADERS
  Value.hpp
  Functions.hpp
  FunctionCall.hpp
  SyntaxTree.hpp
  DataSource.hpp
  StringSource.cpp
  Token.hpp
  Lexer.hpp
  Parser.hpp
)
set(Xavi_SOURCES
  Value.cpp
  Functions.cpp
  FunctionCall.cpp
  SyntaxTree.cpp
  StringSource.cpp
  Token.cpp
  Lexer.cpp
  Parser.cpp
)

add_library(Xavi
  ${Xavi_HEADERS}
  ${Xavi_SOURCES})
target_link_libraries(Xavi
  ${MATH_LIB})
set_target_properties(Xavi
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER ${Xavi_HEADERS})

install(TARGETS Xavi
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE    DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY    DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK  DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}
)
