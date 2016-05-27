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

project(Xavi)
set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0)

#########################
# Configuration Options #
#########################

include(CMakeDependentOption)
option(BUILD_SHARED_LIBS "Build shared libraries" On)
option(BUILD_TESTS "Build testing programs" On)
option(BUILD_CXX "Build C++ Code" On)
option(BUILD_GUI_NATIVE "Build a GUI using native API" On)
cmake_dependent_option(BUILD_GUI_QT "Build a GUI using Qt (Requires C++)" On "BUILD_CXX" Off)

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
if (BUILD_GUI_NATIVE AND APPLE)
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

if (BUILD_GUI_NATIVE AND UNIX AND NOT APPLE)
  set(USE_GTK2 On)
  set(USE_GTKMM2 BUILD_CXX)

  message("-- Finding GTK+ 2")
  find_package(GTK2 QUIET COMPONENTS gtk)
  if(GTK2_FOUND)
    message("-- Finding GTK+ 2 - found")
    if(BUILD_CXX)
      set(TMP_FOUND         ${GTK2_FOUND})
      set(TMP_INCLUDE_DIRS  ${GTK2_INCLUDE_DIRS})
      set(TMP_LIBRARIES     ${GTK2_LIBRARIES})
      set(TMP_DEFINITIONS   ${GTK2_DEFINITIONS})
      set(GTK2_FOUND)
      set(GTK2_INCLUDE_DIRS)
      set(GTK2_LIBRARIES)
      set(GTK2_DEFINITIONS)

      message("-- Finding GTKmm 2")
      find_package(GTK2 QUIET COMPONENTS gtk gtkmm)
      if(GTK2_FOUND)
        message("-- Finding GTKmm 2 - found")
      else()
        message("-- Finding GTKmm 2 - not found")
      endif()

      set(GTKMM2_FOUND ${GTK2_FOUND})
      set(GTKMM2_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS})
      set(GTKMM2_LIBRARIES ${GTK2_LIBRARIES})
      set(GTKMM2_DEFINITIONS ${GTK2_DEFINITIONS})
      set(GTK2_FOUND ${TMP_FOUND})
      set(GTK2_INCLUDE_DIRS ${TMP_INCLUDE_DIRS})
      set(GTK2_LIBRARIES ${TMP_LIBRARIES})
      set(GTK2_DEFINITIONS ${TMP_DEFINITIONS})
      set(TMP_FOUND)
      set(TMP_INCLUDE_DIRS)
      set(TMP_LIBRARIES)
      set(TMP_DEFINITIONS)
    else()
      set(GTKMM2_FOUND No)
    endif()
  else()
    message("-- Finding GTK+ 2 - not found")
    set(GTKMM2_FOUND No)
  endif()
else()
  set(USE_GTK2 Off)
  set(USE_GTKMM2 Off)
  set(GTK2_FOUND No)
  set(GTKMM2_FOUND No)
endif()

if (BUILD_GUI_NATIVE AND WIN32)
  set(USE_USER32 On)
  set(USE_MFC BUILD_CXX)

  message("-- Finding User32.dll")
  find_library(USER32 User32)
  if (NOT ${USER32} STREQUAL "USER32-NOTFOUND")
    message("-- Finding User32.dll - found")
    set(USER32_FOUND Yes)
  else()
    message("-- Finding User32.dll - not found")
    set(USER32_FOUND No)
  endif()

  if(BUILD_CXX)
    message("-- Finding MFC Framework")
    find_package(MFC QUIET)
    if(MFC_FOUND)
      message("-- Finding MFC Framework - found")
    else()
      message("-- Finding MFC Framework - not found")
    endif()
  else()
    set(MFC_FOUND No)
  endif()
else()
  set(USE_USER32 Off)
  set(USE_MFC Off)
  set(USER32_FOUND No)
  set(MFC_FOUND No)
endif()

# Find Qt
if(BUILD_GUI_QT)
    set(USE_QT On)
    message("-- Finding Qt5Widgets")
    find_package(Qt5Widgets QUIET)
    if(Qt5Widgets_FOUND)
        if(APPLE)
          get_target_property(TMP Qt5::Widgets LOCATION)
          string(REPLACE "/QtWidgets.framework/QtWidgets" "" QT_RPATH ${TMP})
          set(TMP)
        endif()
        message("-- Finding Qt5Widgets - found")
    else()
        message("-- Finding Qt5Widgets - not found")
    endif()
else()
  set(USE_QT Off)
  set(Qt5Widgets_FOUND No)
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

if(USE_GTKMM2 AND NOT GTKMM2_FOUND)
    message(FATAL_ERROR "-- Can't build native C++ GUI on Unix. GTKmm 2 not found.")
endif()

if (USE_USER32 AND NOT USER32_FOUND)
  message(FATAL_ERROR "-- Can't build native GUI on Windows. user32.dll not found.")
endif()

if (USE_MFC AND NOT MFC_FOUND)
  message(FATAL_ERROR "-- Can't build native C++ GUI on Windows. MFC not found.")
endif()

if(USE_QT AND NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Can't build Qt GUI. Qt5Widgets not found.")
endif()

############################
# Build-Independent Values #
############################

set(VERSION_STRING "${Xavi_VERSION_MAJOR}.${Xavi_VERSION_MINOR}.${Xavi_VERSION_PATCH}")
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
  set(ARCHIVE_INSTALL_PATH   lib)
  set(LIBRARY_INSTALL_PATH   lib)
  set(BUNDLE_INSTALL_PATH    share/Xavi/error)
  set(FRAMEWORK_INSTALL_PATH share/Xavi/error)
  set(HEADER_INSTALL_PATH    include)
  set(SHARE_INSTALL_PATH     share/Xavi)
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

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Xavi_SOURCE_DIR}/Readme.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Xavi is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Xavi-Cli "Xavi (Command Line)" Xavi-Cli "Xavi (Command Line)")
list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Cli "Xavi (Command Line)" XaviPP-Cli "XaviPP (Command Line)")

if (USE_COCOA)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Cocoa "Xavi (Cocoa GUI)")
  if (BUILD_CXX)
    list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Cocoa "XaviPP (Cocoa GUI)")
  endif()
endif()
if (USE_GTK2)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Gtk "Xavi (Gtk+ GUI)")
endif()
if (USE_GTKMM2)
  list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Gtk "XaviPP (Gtkmm GUI)")
endif()
if (USE_USER32)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Win32 "Xavi (Win32 GUI)")
endif()
if (USE_MFC)
  list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Win32 "XaviPP (MFC GUI)")
endif()
if (USE_QT)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Qt "Xavi (Qt GUI)")
  list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Qt "XaviPP (Qt GUI)")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${Xavi_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Xavi_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Xavi_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Xavi_SOURCE_DIR}/Readme.md")

if(APPLE)
  set(CPACK_GENERATOR DragNDrop)
elseif(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Xavi_SOURCE_DIR}/GPL-3.0.txt")
  if(BUILD_GUI_NATIVE OR BUILD_GUI_QT)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
  endif()
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(Xavi)
if(BUILD_CXX)
  add_subdirectory(XaviPP)
endif()

# Allow executable builds to include library headers.
include_directories(${Xavi_SOURCE_DIR})

# Command-line Programs
add_subdirectory(Xavi-Cli)
if(BUILD_CXX)
  add_subdirectory(XaviPP-Cli)
endif()

# Tests
if(BUILD_TESTS)
  add_subdirectory(Tests)
endif()

# GUI Programs
if(USE_COCOA)
  add_subdirectory(Xavi-Gui-Cocoa)
endif()

if(USE_COCOA AND BUILD_CXX)
  add_subdirectory(XaviPP-Gui-Cocoa)
endif()

if(USE_GTK)
  add_subdirectory(Xavi-Gui-Gtk)
endif()

if(USE_GTKMM AND BUILD_CXX)
  add_subdirectory(XaviPP-Gui-Gtk)
endif()

if(USE_WIN32)
    add_subdirectory(Xavi-Gui-Win32)
endif()

if(USE_MFC AND BUILD_CXX)
    add_subdirectory(XaviPP-Gui-Win32)
endif()

if(USE_QT)
  add_subdirectory(Xavi-Gui-Qt)
  add_subdirectory(XaviPP-Gui-Qt)
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
