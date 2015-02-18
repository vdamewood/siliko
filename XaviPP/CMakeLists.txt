# XaviPP

set(XaviPP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

set(XaviPP_PUBLIC_HEADERS
  W32Dll.hpp
  Value.hpp
  Functions.hpp
  FunctionCaller.hpp
  SyntaxTree.hpp
  DataSource.hpp
  Token.hpp
  Lexer.hpp
  InfixParser.hpp
)

set(XaviPP_HEADERS
  ${XaviPP_PUBLIC_HEADERS}
)

set(XaviPP_SOURCES
  Value.cpp
  Functions.cpp
  FunctionCaller.cpp
  SyntaxTree.cpp
  Token.cpp
  Lexer.cpp
  InfixParser.cpp
)

add_library(XaviPP
  ${XaviPP_HEADERS}
  ${XaviPP_SOURCES})
set_target_properties(XaviPP
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER "${XaviPP_PUBLIC_HEADERS}")

install(TARGETS XaviPP
  RUNTIME       DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE       DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY       DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK     DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}/XaviPP
)
