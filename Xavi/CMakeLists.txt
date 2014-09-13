# Xavi

if (UNIX)
	add_definitions(-DXAVI_USE_UNIX=1)
else()
	add_definitions(-DXAVI_USE_UNIX=0)
endif()

set(Xavi_PUBLIC_HEADERS Xavi.h)
set(Xavi_INTERNAL_HEADERS
  FunctionCall.hpp
  SyntaxTree.hpp XaviValue.hpp
  Parser.hpp Lexer.hpp
  DataSource.hpp CStringSource.cpp)
set(Xavi_SOURCES
  Xavi.cpp
  FunctionCall.cpp
  SyntaxTree.cpp
  Parser.cpp Lexer.cpp
  CStringSource.cpp)

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
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE    DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY    DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK  DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}
)
