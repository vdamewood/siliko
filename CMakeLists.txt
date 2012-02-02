# Root
cmake_minimum_required (VERSION 2.6)
project(Xavi)
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
option(BUILD_TESTS "Build testing programs." ON)

if(UNIX AND NOT APPLE)
    SET(MATH_LIB "m")
else(UNIX AND NOT APPLE)
    SET(MATH_LIB )
endif(UNIX AND NOT APPLE)

add_subdirectory(Xavi)
if(BUILD_XAVISTA)
  add_subdirectory(Xavista)
endif(BUILD_GUI)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif(BUILD_TESTS)
