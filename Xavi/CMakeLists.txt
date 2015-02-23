# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015 Vincent Damewood
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

set(LIBNAME Xavi)
set(PHDRS
  W32Dll.h
  Value.h
  FunctionCaller.h
  SyntaxTree.h
  DataSource.h
  StringSource.h
  Lexer.h
  InfixParser.h)
set(HDRS
  ${PHDRS}
  Functions.h
  Hash.h)
set(SRCS
  Hash.c
  Functions.c
  FunctionCaller.c
  SyntaxTree.c
  DataSource.c
  StringSource.c
  Lexer.c
  InfixParser.c)
set(LIBS ${MATH})

add_library("${LIBNAME}" ${HDRS} ${SRCS})
target_link_libraries("${LIBNAME}" ${LIBS})
set_target_properties("${LIBNAME}" PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER "${PHDRS}")
install(TARGETS ${LIBNAME}
  RUNTIME       DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE       DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY       DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK     DESTINATION ${FRAMEWORK_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}/${LIBNAME})

set(CLIB ${LIBNAME} PARENT_SCOPE)
set(CLIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
