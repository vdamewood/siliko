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

###########################
# Preamble (Semi-Requied) #
###########################

if(APPLE)
    cmake_minimum_required(VERSION 3.1.0)
else()
    cmake_minimum_required(VERSION 2.8.6)
endif()

project(Xavi)
set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0)

#######################
# Dependency Searches #
#######################


# Detect if system has separate math library (libm)
message("-- Finding Math Library")
find_library(MATH m)
if(${MATH} STREQUAL "MATH-NOTFOUND")
    message("-- Finding Math Library - not needed")
    set(MATH "")
else()
    message("-- Finding Math Library - found")
endif()

# Find Cocoa and ibtool
if(APPLE)
  message("-- Finding Cocoa")
  find_library(COCOA Cocoa)
  if(NOT ${COCOA} STREQUAL "COCOA-NOTFOUND")
    message("-- Finding Cocoa - found")
    set(COCOA_FOUND Yes)
  else()
    message("-- Finding Cocoa - not found")
    set(COCOA_FOUND No)
  endif()

  message("-- Finding ibtool")
  find_program(IBTOOL ibtool)
  if (NOT ${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
    message("-- Finding ibtool - found")
    set(IBTOOL_FOUND Yes)
  else()
    message("-- Finding ibtool - not found")
    set(IBTOOL_FOUND No)
  endif()
else()
  set(IBTOOL_FOUND No)
  set(COCOA_FOUND No)
endif()

# Find User32.dll
if(WIN32)
  message("-- Finding User32.dll")
  find_library(USER32 User32)
  if (NOT ${USER32} STREQUAL "USER32-NOTFOUND")
    message("-- Finding User32.dll - found")
    set(USER32_FOUND Yes)
  else()
    message("-- Finding User32.dll - not found")
    set(USER32_FOUND No)
  endif()
else()
  set(USER32_FOUND No)
endif()

if(UNIX)
  message("-- Finding GTK+ 2")
  find_package(GTK2 QUIET COMPONENTS gtk glade)
  if(GTK2_FOUND)
    message("-- Finding GTK+ 2 - found")
    set(TMP_FOUND         ${GTK2_FOUND})
    set(TMP_INCLUDE_DIRS  ${GTK2_INCLUDE_DIRS})
    set(TMP_LIBRARIES     ${GTK2_LIBRARIES})
    set(TMP_DEFINITIONS   ${GTK2_DEFINITIONS})
    set(GTK2_FOUND)
    set(GTK2_INCLUDE_DIRS)
    set(GTK2_LIBRARIES)
    set(GTK2_DEFINITIONS)

    message("-- Finding GTKmm 2")
    find_package(GTK2 QUIET COMPONENTS gtk glade gtkmm glademm)
    if(GTK2_FOUND)
      message("-- Finding GTKmm 2 - found")
      set(GTKMM2_FOUND ${GTK2_FOUND})
      set(GTKMM2_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS})
      set(GTKMM2_LIBRARIES ${GTK2_LIBRARIES})
      set(GTKMM2_DEFINITIONS ${GTK2_DEFINITIONS})
    else()
      message("-- Finding GTKmm 2 - not found")
    endif()

    set(GTK2_FOUND ${TMP_FOUND})
    set(GTK2_INCLUDE_DIRS ${TMP_INCLUDE_DIRS})
    set(GTK2_LIBRARIES ${TMP_LIBRARIES})
    set(GTK2_DEFINITIONS ${TMP_DEFINITIONS})
    set(TMP_FOUND)
    set(TMP_INCLUDE_DIRS)
    set(TMP_LIBRARIES)
    set(TMP_DEFINITIONS)
  else()
    message("-- Finding GTK+ 2 - not found")
    set(GTKMM2_FOUND No)
  endif()
endif()

# Find Qt
if(${CMAKE_VERSION} VERSION_LESS "2.8.11")
    set(Qt5Widgets_POSSIBLE No)
    set(Qt5Widgets_FOUND No)
    set(BUILD_QT_GUI No)
    message("-- Can't build with Qt5 (Needs Cmake 4.8.11 or later). Skipping.")
else()
    set(Qt5Widgets_POSSIBLE On)
    message("-- Finding Qt5Widgets")
    find_package(Qt5Widgets QUIET)
    if(${Qt5Widgets_FOUND})
        message("-- Finding Qt5Widgets - found")
    else()
        message("-- Finding Qt5Widgets - not found")
    endif()
endif()

#########################
# Configuration Options #
#########################

# CMake Standard
option(CMAKE_BUILD_TYPE        "Build as Debug or Release."            Debug)
option(BUILD_SHARED_LIBS       "Build libraries as shared libraries."  On)
option(CMAKE_VERBOSE_MAKEFILES "Verbose output from makefiles."        Off)

# Optional Components
option(BUILD_TESTS             "Build testing programs."               On)
option(BUILD_PLATFORM_GUI      "Build a native GUI for this platform." On)
if(Qt5Widgets_POSSIBLE)
  option(BUILD_QT_GUI          "Build a Qt GUI."                       ${Qt5Widgets_FOUND})
endif()

# Enable Platform GUIs
if (BUILD_PLATFORM_GUI AND APPLE)
    set(BUILD_COCOA On)
else()
    set(BUILD_COCOA Off)
endif()

if (BUILD_PLATFORM_GUI AND UNIX)
    set(BUILD_GTK On)
else()
    set(BUILD_GTK Off)
endif()

if (BUILD_PLATFORM_GUI AND WIN32)
    set(BUILD_WIN32 On)
else()
    set(BUILD_WIN32 Off)
endif()

# This is redundant, but keeps things consistent.
set(BUILD_QT ${BUILD_QT_GUI})

####################
# Failure Messages #
####################

if(${BUILD_COCOA} AND NOT ${IBTOOL_FOUND})
  message(FATAL_ERROR "Can't build native GUI on OS X: ibtool not found.")
endif()

if(${BUILD_GTK})
  if(NOT ${GTK2_FOUND})
    message(FATAL_ERROR "Can't build native GUI on Unix: GTK 2 not found.")
  elseif(NOT ${GTKMM2_FOUND})
    message(FATAL_ERROR "Can't build native GUI on Unix: GTKmm 2 not found.")
  endif()
endif()

if(${BUILD_QT} AND NOT ${Qt5Widgets_FOUND})
  message(FATAL_ERROR "Can't build Qt GUI: Qt5Widgets not found.")
endif()

############################
# Build-Independent Values #
############################

set(VERSION_STRING "${Xavi_VERSION_MAJOR}.${Xavi_VERSION_MINOR}.${Xavi_VERSION_PATCH}")
set(LIB_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015 Vincent Damewood. Licensed under the terms of the GNU Lesser General Public License, version 3 or later.")
set(APP_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015 Vincent Damewood. Licensed under the terms of the GNU General Public License, version 3 or later.")
set(BUNDLE_ID_DOMAIN "com.vdamewood")

###########################
# System-dependent Values #
###########################

if(WIN32)
  set(RUNTIME_INSTALL_PATH   .)
  set(ARCHIVE_INSTALL_PATH   .)
  set(LIBRARY_INSTALL_PATH   .)
  set(BUNDLE_INSTALL_PATH    Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH    ./include)
  set(DOC_INSTALL_PATH       .)
elseif(APPLE)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   Error)
  set(LIBRARY_INSTALL_PATH   Error)
  set(BUNDLE_INSTALL_PATH    .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH    Error)
  set(DOC_INSTALL_PATH       .)
elseif(UNIX)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   lib)
  set(LIBRARY_INSTALL_PATH   lib)
  set(BUNDLE_INSTALL_PATH    share/Xavi/error)
  set(FRAMEWORK_INSTALL_PATH share/Xavi/error)
  set(HEADER_INSTALL_PATH    include)
  set(DOC_INSTALL_PATH       share/Xavi/docs)
endif()

#############################
# Compiler-dependent values #
#############################


if (${CMAKE_C_COMPILER_ID} STREQUAL "Clang"
  OR ${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang"
  OR ${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "SunPro")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "SunPro")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Xavi_SOURCE_DIR}/Readme.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Xavi is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Xavi-Cli "Xavi (Command Line)" XaviPP-Cli "XaviPP (Command Line)")
if (${BUILD_WIN32})
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Win32 "Xavi (Win32 GUI)")
endif()
if (${BUILD_QT})
  list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Qt "XaviPP (Qt GUI)")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR ${Xavi_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Xavi_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Xavi_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Xavi_SOURCE_DIR}/Readme.txt")


if(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Xavi_SOURCE_DIR}/GPL-3.0.txt")
  if(BUILD_QT OR BUILD_WIN32)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
  endif()
elseif(APPLE)
  set(CPACK_GENERATOR DragNDrop)
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(Xavi)
add_subdirectory(XaviPP)

# Allow executable builds to include library headers.
include_directories(${Xavi_SOURCE_DIR})

# Command-line Programs
add_subdirectory(Xavi-Cli)
add_subdirectory(XaviPP-Cli)

# Tests
if(${BUILD_TESTS})
  add_subdirectory(Tests)
endif()

# GUI Programs
if(${BUILD_COCOA})
  add_subdirectory(Xavi-Gui-Cocoa)
  add_subdirectory(XaviPP-Gui-Cocoa)
endif()

if(${BUILD_WIN32})
    add_subdirectory(Xavi-Gui-Win32)
#    add_subdirectory(XaviPP-Gui-Win32)
endif()

if(${BUILD_GTK})
  add_subdirectory(Xavi-Gui-Gtk)
#  add_subdirectory(XaviPP-Gui-Gtk)
endif()

if(${BUILD_QT})
#    add_subdirectory(Xavi-Gui-Qt)
    add_subdirectory(XaviPP-Gui-Qt)
endif()

##################################
# Preexisting File Installations #
##################################

install(
  FILES Readme.txt Authors.txt GPL-3.0.txt LGPL-3.0.txt
  DESTINATION ${DOC_INSTALL_PATH})

###############################################
# CPack Inclusion (Used for package creation) #
###############################################

include(CPack)
