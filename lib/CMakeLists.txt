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

set(TARGET SilikoLib)
set(PUB_HEADERS
  W32Dll.h
  Value.h
  FunctionCaller.h
  SyntaxTree.h
  DataSource.h
  StringSource.h
  Token.h
  Lexer.h
  InfixParser.h)
set(HEADERS
  ${PUB_HEADERS}
  Functions.h
  Hash.h)
set(SOURCES
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
set(DEPS "")

# Detect if system has separate math library (libm)
message("-- Finding math library")
find_library(MATHLIB m)
if(${MATHLIB} STREQUAL "MATHLIB-NOTFOUND")
    message("-- Finding math library - not needed")
else()
    message("-- Finding math library - found")
    list(APPEND DEPS ${MATHLIB})
endif()

add_library(${TARGET} ${HEADERS} ${SOURCES} ${RESOURCES})
set_target_properties(${TARGET} PROPERTIES
    OUTPUT_NAME   ${ONAMEDLIB}
    FRAMEWORK     ${BUILD_MACOSX}
    PUBLIC_HEADER ${PUB_HEADERS}
    RESOURCE      ${RESOURCES}
    VERSION       ${PROJECT_VERSION}
    SOVERSION     ${PROJECT_VERSION_MAJOR})
target_link_libraries(${TARGET} ${DEPS})

install(TARGETS ${TARGET}
  RUNTIME       DESTINATION ${INSTBIN}
  ARCHIVE       DESTINATION ${INSTLIB}
  LIBRARY       DESTINATION ${INSTLIB}
  FRAMEWORK     DESTINATION .
  RESOURCE      DESTINATION ${INSTDAT}
  PUBLIC_HEADER DESTINATION ${INSTINC}/Siliko)

set(LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
