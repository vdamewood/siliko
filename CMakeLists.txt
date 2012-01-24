# Root
cmake_minimum_required (VERSION 2.6)
project(XVCalc)
set(XVC_VERSION_MAJOR 0)
set(XVC_VERSION_MINOR 0)
set(XVC_VERSION_PATCH 0)

find_package(BISON 2.4.2 REQUIRED)
find_package(FLEX 2.5.35 REQUIRED)
find_package(Qt4)

option(BUILD_GUI "Build a GUI frontend to XVCalc" ${QT4_FOUND})
option(BUILD_SHARED_LIBS "Build shared library." ON)
option(BUILD_TESTS "Build testing programs." ON)

if(UNIX AND NOT APPLE)
    SET(MATH_LIB "m")
else(UNIX AND NOT APPLE)
    SET(MATH_LIB )
endif(UNIX AND NOT APPLE)

add_subdirectory(libxvcalc)
if(BUILD_GUI)
  add_subdirectory(XvcGui)
endif(BUILD_GUI)
if(BUILD_TESTS)
  add_subdirectory(tests)
endif(BUILD_TESTS)
