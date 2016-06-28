# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

set(LIBNAME Siliko)
set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 0)
set(LIB_VERSION_PATCH 0)
set(LIB_VERSION_STRING "${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}")

set(PHDRS
  W32Dll.h
  Value.h
  FunctionCaller.h
  SyntaxTree.h
  DataSource.h
  StringSource.h
  Token.h
  Lexer.h
  InfixParser.h)
set(HDRS
  ${PHDRS}
  Functions.h
  Hash.h)
set(SRCS
  Value.c
  Hash.c
  Functions.c
  FunctionCaller.c
  SyntaxTree.c
  DataSource.c
  StringSource.c
  Lexer.c
  InfixParser.c)
set(RESOURCES
  SilikoConfig.cmake)

# Detect if system has separate math library (libm)
message("-- Finding Math Library")
find_library(MATH m)
if(${MATH} STREQUAL "MATH-NOTFOUND")
    message("-- Finding Math Library - not needed")
    set(LIBS "")
else()
    message("-- Finding Math Library - found")
    set(LIBS ${MATH})
endif()

add_library("${LIBNAME}" ${HDRS} ${SRCS} ${RESOURCES})
target_link_libraries("${LIBNAME}" ${LIBS})
set_target_properties("${LIBNAME}" PROPERTIES
    FRAMEWORK     TRUE
    PUBLIC_HEADER "${PHDRS}"
    RESOURCE      "${RESOURCES}"
    VERSION       ${LIB_VERSION_STRING}
    SOVERSION     ${LIB_VERSION_MAJOR})
install(TARGETS ${LIBNAME}
  RUNTIME       DESTINATION ${RUNTIME_INSTALL_PATH}
  ARCHIVE       DESTINATION ${ARCHIVE_INSTALL_PATH}
  LIBRARY       DESTINATION ${LIBRARY_INSTALL_PATH}
  FRAMEWORK     DESTINATION ${FRAMEWORK_INSTALL_PATH}
  RESOURCE      DESTINATION ${SHARE_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_PATH}/${LIBNAME})

set(LIB ${LIBNAME} PARENT_SCOPE)
set(LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
