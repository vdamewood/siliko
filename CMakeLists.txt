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

############################
# Preamble (Semi-Required) #
############################

cmake_minimum_required(VERSION 3.1.0)

project(Siliko C)
set(Siliko_VERSION_MAJOR 0)
set(Siliko_VERSION_MINOR 0)
set(Siliko_VERSION_PATCH 0)

#########################
# Configuration Options #
#########################

option(BUILD_SHARED_LIBS "Build shared libraries" On)
option(BUILD_TESTS       "Build testing programs" On)
option(BUILD_GUI         "Build a GUI Program"    On)

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
if (BUILD_GUI AND APPLE)
  set(USE_COCOA On)

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
  set(USE_COCOA Off)
  set(COCOA_FOUND No)
  set(IBTOOL_FOUND No)
endif()

if (BUILD_GUI AND UNIX AND NOT APPLE)
  set(USE_GTK2 On)

  message("-- Finding GTK+ 2")
  find_package(GTK2 QUIET COMPONENTS gtk)
  if(GTK2_FOUND)
    message("-- Finding GTK+ 2 - found")
  else()
    message("-- Finding GTK+ 2 - not found")
  endif()
else()
  set(USE_GTK2 Off)
  set(GTK2_FOUND No)
endif()

if (BUILD_GUI AND WIN32)
  set(USE_USER32 On)

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
  set(USE_USER32 Off)
  set(USER32_FOUND No)
endif()

####################
# Failure Messages #
####################

if(USE_COCOA AND NOT COCOA_FOUND)
  message(FATAL_ERROR "Can't build native GUI on OS X. Cocoa not found.")
endif()

if(USE_COCOA AND NOT IBTOOL_FOUND)
  message(FATAL_ERROR "Can't build native GUI on OS X. ibtool not found.")
endif()

if(USE_GTK2 AND NOT GTK2_FOUND)
    message(FATAL_ERROR "Can't build native GUI on Unix. GTK 2 not found.")
endif()

if (USE_USER32 AND NOT USER32_FOUND)
  message(FATAL_ERROR "-- Can't build native GUI on Windows. user32.dll not found.")
endif()

############################
# Build-Independent Values #
############################

set(VERSION_STRING "${Siliko_VERSION_MAJOR}.${Siliko_VERSION_MINOR}.${Siliko_VERSION_PATCH}")
set(LIB_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016 Vincent Damewood. Licensed under the terms of the GNU Lesser General Public License, version 3 or later.")
set(APP_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016 Vincent Damewood. Licensed under the terms of the GNU General Public License, version 3 or later.")
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
  set(SHARE_INSTALL_PATH     .)
  set(DOC_INSTALL_PATH       .)
elseif(APPLE)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   Error)
  set(LIBRARY_INSTALL_PATH   Error)
  set(BUNDLE_INSTALL_PATH    .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH    Error)
  set(SHARE_INSTALL_PATH     Error)
  set(DOC_INSTALL_PATH       .)
elseif(UNIX)
  set(RUNTIME_INSTALL_PATH   bin)
  if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(ARCHIVE_INSTALL_PATH   lib64)
    set(LIBRARY_INSTALL_PATH   lib64)
  else()
    set(ARCHIVE_INSTALL_PATH   lib)
    set(LIBRARY_INSTALL_PATH   lib)
  endif()
  set(BUNDLE_INSTALL_PATH    share/Siliko/error)
  set(FRAMEWORK_INSTALL_PATH share/Siliko/error)
  set(HEADER_INSTALL_PATH    include)
  set(SHARE_INSTALL_PATH     share/Siliko)
  set(DOC_INSTALL_PATH       share/Siliko/docs)
endif()


#############################
# Compiler-dependent values #
#############################


if (${CMAKE_C_COMPILER_ID} STREQUAL "Clang"
  OR ${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang"
  OR ${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
endif()

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Siliko_SOURCE_DIR}/Readme.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Siliko is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Siliko-Cli "Siliko (Command Line)" Siliko-Cli "Siliko (Command Line)")
list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Cli "Siliko (Command Line)" SilikoPP-Cli "SilikoPP (Command Line)")

if (USE_COCOA)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Gui-Cocoa "Siliko (Cocoa GUI)")
endif()

if (USE_GTK2)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Gui-Gtk "Siliko (Gtk+ GUI)")
endif()

if (USE_USER32)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Gui-Win32 "Siliko (Win32 GUI)")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${Siliko_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Siliko_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Siliko_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Siliko_SOURCE_DIR}/Readme.md")

if(APPLE)
  set(CPACK_GENERATOR DragNDrop)
elseif(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Siliko_SOURCE_DIR}/GPL-3.0.txt")
  if(BUILD_GUI_NATIVE OR BUILD_GUI_QT)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
  endif()
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(lib)

# Allow executable builds to include library headers.
include_directories("${Siliko_SOURCE_DIR}/lib")

# Command-line Programs
add_subdirectory(cli)

# Tests
if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

# GUI Programs
if(USE_COCOA)
  add_subdirectory(gui-osx)
endif()

if(USE_GTK2)
  add_subdirectory(gui-unix)
endif()

if(USE_USER32)
    add_subdirectory(gui-windows)
endif()

##################################
# Preexisting File Installations #
##################################

install(
  FILES Readme.md Authors.txt GPL-3.0.txt LGPL-3.0.txt
  DESTINATION ${DOC_INSTALL_PATH})

###############################################
# CPack Inclusion (Used for package creation) #
###############################################

include(CPack)
