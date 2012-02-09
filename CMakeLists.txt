# Root
project(Xavi)
cmake_minimum_required (VERSION 2.8)

set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0)

find_package(BISON 2.4.1 REQUIRED)
find_package(FLEX 2.5.35 REQUIRED)
find_package(Qt4)

option(BUILD_XAVISTA
  "Build Xavista, a Qt-based GUI frontend to Xavi"
  ${QT4_FOUND})
option(BUILD_SHARED_LIBS "Build Xavi as a shared library." ON)
option(BUILD_TESTS "Build testing programs." OFF)

if(UNIX AND NOT APPLE)
    SET(MATH_LIB "m")
else(UNIX AND NOT APPLE)
    SET(MATH_LIB )
endif(UNIX AND NOT APPLE)

add_subdirectory(Xavi)
if(BUILD_XAVISTA)
  add_subdirectory(Xavista)
endif(BUILD_XAVISTA)
if(BUILD_TESTS)
  add_subdirectory(Tests)
endif(BUILD_TESTS)

set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)
