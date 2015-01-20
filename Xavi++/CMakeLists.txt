# Xavi

if (UNIX)
	add_definitions(-DXAVI_USE_UNIX=1)
else()
	add_definitions(-DXAVI_USE_UNIX=0)
endif()

set(Xavi++_PUBLIC_HEADERS
  W32Dll.hpp
  Value.hpp
  Functions.hpp
  FunctionCaller.hpp
  SyntaxTree.hpp
  DataSource.hpp
  CStringSource.cpp
  Token.hpp
  Lexer.hpp
  InfixParser.hpp
)

set(Xavi++_HEADERS
  ${XAVI_PUBLIC_HEADERS}
  Hash.cpp)

set(Xavi++_SOURCES
  Hash.cpp
  Value.cpp
  Functions.cpp
  FunctionCaller.cpp
  SyntaxTree.cpp
  CStringSource.cpp
  Token.cpp
  Lexer.cpp
  InfixParser.cpp
)

add_library(Xavi++
  ${Xavi++_HEADERS}
  ${Xavi++_SOURCES})
target_link_libraries(Xavi++
  ${MATH_LIB})
set_target_properties(Xavi++
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER "${Xavi_PUBLIC_HEADERS}")

install(TARGETS Xavi++
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE    DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY    DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK  DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}
)
