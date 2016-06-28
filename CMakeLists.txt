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

option(BUILD_SHARED_LIBS "Build shared libraries"          On)
option(BUILD_CLI         "Build command-line application." On)
option(BUILD_GUI         "Build a GUI Program"             On)
if(APPLE)
  option(FORCE_UNIX      "Force Unix-style installation (Macintosh OS X Only)" Off)
endif()

###############
# Build Style #
###############

if(APPLE)
  if(FORCE_UNIX)
    set(BUILD_UNIX On)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOSX Off)
  else()
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOSX On)
  endif()
elseif(UNIX)
    set(BUILD_UNIX On)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOSX Off)
elseif(WIN32)
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS On)
    set(BUILD_MACOSX Off)
else()
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOSX Off)
endif()

############################
# Build-Independent Values #
############################

set(VERSION_STRING      "${Siliko_VERSION_MAJOR}.${Siliko_VERSION_MINOR}.${Siliko_VERSION_PATCH}")
set(COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016 Vincent Damewood.\
 Siliko library licensed under the terms of the GNU Lesser General Public License, version 3 or later.\
 Siliko interface programs licensed under the terms of the GNU General Public License, version 3 or later.")

###########################
# System-dependent Values #
###########################

if(BUILD_WINDOWS)
  set(RUNTIME_INSTALL_PATH   .)
  set(ARCHIVE_INSTALL_PATH   .)
  set(LIBRARY_INSTALL_PATH   .)
  set(BUNDLE_INSTALL_PATH    Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH    ./include)
  set(SHARE_INSTALL_PATH     .)
  set(DOC_INSTALL_PATH       .)
elseif(BUILD_MACOSX)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   Error)
  set(LIBRARY_INSTALL_PATH   Error)
  set(BUNDLE_INSTALL_PATH    .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH    Error)
  set(SHARE_INSTALL_PATH     Error)
  set(DOC_INSTALL_PATH       .)
else()
  include(GNUInstallDirs)
  set(RUNTIME_INSTALL_PATH   ${CMAKE_INSTALL_BINDIR})
  set(ARCHIVE_INSTALL_PATH   ${CMAKE_INSTALL_LIBDIR})
  set(LIBRARY_INSTALL_PATH   ${CMAKE_INSTALL_LIBDIR})
  set(BUNDLE_INSTALL_PATH    "${CMAKE_INSTALL_DATADIR}/Siliko/error")
  set(FRAMEWORK_INSTALL_PATH "${CMAKE_INSTALL_DATADIR}/Siliko/error")
  set(HEADER_INSTALL_PATH    ${CMAKE_INSTALL_INCLUDEDIR})
  set(SHARE_INSTALL_PATH     "${CMAKE_INSTALL_DATADIR}/Siliko")
  set(DOC_INSTALL_PATH       ${CMAKE_INSTALL_DOCDIR})
endif()

#############################
# Compiler-dependent values #
#############################

set(COMPILER_LIST Clang AppleClang GNU)
list(FIND COMPILER_LIST ${CMAKE_C_COMPILER_ID} COMPILER_INDEX)
if(NOT ${COMPILER_INDEX} LESS 0)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
endif()

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Siliko_SOURCE_DIR}/Readme.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Siliko is a mathematical expression parser library.")

set(CPACK_PACKAGE_EXECUTABLES)
if(BUILD_CLI)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Cli "Siliko (Command Line)")
endif()
if(BUILD_GUI)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Siliko-Gui "Siliko (GUI)")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${Siliko_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Siliko_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Siliko_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/" "/build/")
set(CPACK_RESOURCE_FILE_README "${Siliko_SOURCE_DIR}/Readme.md")

if(BUILD_MACOSX)
  set(CPACK_GENERATOR DragNDrop)
elseif(BUILD_WINDOWS)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Siliko_SOURCE_DIR}/GPL-3.0.txt")
  if(BUILD_GUI)
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
if(BUILD_CLI)
  add_subdirectory(command)
endif()

# GUI Programs
if(BUILD_GUI)
  if(BUILD_MACOSX)
    add_subdirectory(cocoa)
  elseif(BUILD_UNIX)
    add_subdirectory(gtk2)
  elseif(BUILD_WINDOWS)
    add_subdirectory(win32)
  else()
    message(FATAL_ERROR "A GUI program is not available for this system.")
  endif()
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
