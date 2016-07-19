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

set(DTARGET SilikoDLib)
set(STARGET SilikoSLib)

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

# Detect if system has separate math library (libm)
set(NEED_LM Off)
include(CheckSymbolExists)
check_symbol_exists(sin math.h FOUND_SIN)
if(NOT "${FOUND_SIN}")
  set(CMAKE_REQUIRED_LIBRARIES m)
  check_symbol_exists(sin math.h FOUND_SIN_WITH_LM)
  if(${FOUND_SIN_WITH_LM})
    set(NEED_LM On)
  endif()
endif()

if(BUILD_DLIB)
  add_library(${DTARGET} SHARED ${HEADERS} ${SOURCES} ${RESOURCES})
  set_target_properties(${DTARGET} PROPERTIES
      OUTPUT_NAME   ${ONAMEDLIB}
      FRAMEWORK     ${BUILD_MACOSX}
      PUBLIC_HEADER "${PUB_HEADERS}"
      RESOURCE      "${RESOURCES}"
      VERSION       ${PROJECT_VERSION}
      SOVERSION     ${PROJECT_VERSION_MAJOR})
  if("${NEED_M}")
    target_link_libraries(${DTARGET} m)
  endif()

  install(TARGETS ${DTARGET}
    RUNTIME       DESTINATION ${INSTBIN}
    ARCHIVE       DESTINATION ${INSTLIB}
    LIBRARY       DESTINATION ${INSTLIB}
    FRAMEWORK     DESTINATION .
    RESOURCE      DESTINATION ${INSTDAT}
    PUBLIC_HEADER DESTINATION ${INSTINC}/Siliko)
endif()

if(BUILD_SLIB)
  add_library(${STARGET} STATIC ${HEADERS} ${SOURCES} ${RESOURCES})
  target_compile_definitions(${STARGET}
      PUBLIC        SILIKO_STATIC=1)
  set_target_properties(${STARGET} PROPERTIES
      OUTPUT_NAME   ${ONAMESLIB}
      RESOURCE      ${RESOURCES}
      VERSION       ${PROJECT_VERSION}
      SOVERSION     ${PROJECT_VERSION_MAJOR})
  if("${NEED_M}")
    target_link_libraries(${STARGET} m)
  endif()

  install(TARGETS ${STARGET}
    ARCHIVE       DESTINATION ${INSTLIB}
    RESOURCE      DESTINATION ${INSTDAT}
    PUBLIC_HEADER DESTINATION ${INSTINC}/Siliko)

  install(FILES ${PUB_HEADERS}
    DESTINATION ${INSTINC}/Siliko)
endif()

if(BUILD_UNIX)
  configure_file(Siliko.pc.in Siliko.pc @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Siliko.pc" DESTINATION "${INSTLIB}/pkgconfig")
endif()

if(BUILD_DLIB)
  set(LIB "${DTARGET}" PARENT_SCOPE)
else()
  set(LIB "${STARGET}" PARENT_SCOPE)
endif()

set(LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
