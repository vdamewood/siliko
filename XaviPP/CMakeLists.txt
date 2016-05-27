# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, Vincent Damewood
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

set(LIBNAME XaviPP)
set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 0)
set(LIB_VERSION_PATCH 0)
set(LIB_VERSION_STRING "${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}")

set(PHDRS
  W32Dll.hpp
  Value.hpp
  FunctionCaller.hpp
  SyntaxTree.hpp
  DataSource.hpp
  CStringSource.hpp
  Token.hpp
  Lexer.hpp
  InfixParser.hpp)
set(HDRS
  ${PHDRS}
  Functions.hpp)
set(SRCS
  Value.cpp
  Functions.cpp
  FunctionCaller.cpp
  SyntaxTree.cpp
  CStringSource.cpp
  Token.cpp
  Lexer.cpp
  InfixParser.cpp)

add_library(${LIBNAME} ${HDRS} ${SRCS})
set_target_properties(${LIBNAME}
  PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER "${PHDRS}"
    VERSION       ${LIB_VERSION_STRING}
    SOVERSION     ${LIB_VERSION_MAJOR})
install(TARGETS XaviPP
  RUNTIME       DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE       DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY       DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK     DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}/${LIBNAME})

set(CXXLIB ${LIBNAME} PARENT_SCOPE)
set(CXXLIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
