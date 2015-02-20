# Root

###########################
# Preamble (Semi-Requied) #
###########################

cmake_minimum_required(VERSION 2.8.11)

project(Xavi)
set(Xavi_VERSION_MAJOR 0)
set(Xavi_VERSION_MINOR 0)
set(Xavi_VERSION_PATCH 0)


# These are values shared by multiple subdirectories.
set(Xavi_VERSION_STRING "${XAVI_VERSION_MAJOR}.${XAVI_VERSION_MINOR}.${XAVI_VERSION_PATCH}")
set(Xavi_APP_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015 Vincent Damewood. Licensed under the terms of the GNU General Public License, version 3 or later.")
set(CMAKE_VERBOSE_MAKEFILE On)

#######################
# Dependency Searches #
#######################

# ibtool
if(APPLE)
  message("-- Finding ibtool")
  find_program(IBTOOL ibtool)
  if (NOT ${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
    message("-- Finding ibtool: - found")
    set(IBTOOL_FOUND Yes)
  else()
    message("-- Finding ibtool: - not found")
    set(IBTOOL_FOUND No)
  endif()
else()
  set(IBTOOL_FOUND No)
endif()

# Qt
message("-- Finding Qt5Widgets")
find_package(Qt5Widgets QUIET)
if(Qt5Widgets_FOUND)
  message("-- Finding Qt5Widgets - found")
else()
  message("-- Finding Qt5Widgets - not found")
endif()

#########################
# Configuration Options #
#########################

# CMake Standard
option(CMAKE_BUILD_TYPE       "Build as Debug or Release."                         Debug)
option(BUILD_SHARED_LIBS      "Build Xavi and XaviPP as shared libraries."         On)

# GUI frontends
option(BUILD_TESTS            "Build testing programs."                            On)
option(BUILD_XAVI_GUI_COCOA   "Build a GUI based on Cocoa for Xavi (OS X Only)."   ${APPLE})
option(BUILD_XAVIPP_GUI_COCOA "Build a GUI based on Cocoa for XaviPP (OS X Only)." ${APPLE})
option(BUILD_XAVIPP_GUI_QT    "Build a Qt-based GUI frontend to XaviPP."           ${Qt5Widgets_FOUND})
option(BUILD_XAVI_GUI_WIN32   "Build GUI based on Win32 API (Windows Only)."       ${WIN32})

####################
# Failure Messages #
####################

if(${BUILD_XAVI_GUI_COCOA} OR ${BUILD_XAVIPP_GUI_COCOA} AND NOT ${IBTOOL_FOUND})
  message(FATAL_ERROR "Can't build Xavi-Gui-Cocoa or XaviPP-Gui-Cocoa: ibtool not found.")
endif()

if(${BUILD_XAVIPP_GUI_QT} AND NOT ${Qt5Widgets_FOUND})
  message(FATAL_ERROR "Can't build XaviPP-GUI-Qt: Qt5Widgets not found.")
endif()

if(${BUILD_XAVI_GUI_WIN32} AND NOT WIN32)
  message(FATAL_ERROR "Can't build XaviPP-GUI-Win32: not building on Windows.")
endif()

###########################
# System-dependent Values #
###########################

if(WIN32)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH   .)
  set(ARCHIVE_INSTALL_PATH   .)
  set(LIBRARY_INSTALL_PATH   .)
  set(BUNDLE_INSTALL_PATH    Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH    ./include)
  set(DOC_INSTALL_PATH       .)
elseif(APPLE)
  set(MATH_LIB)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   Error)
  set(LIBRARY_INSTALL_PATH   Error)
  set(BUNDLE_INSTALL_PATH    .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH    Error)
  set(DOC_INSTALL_PATH       .)
elseif(UNIX)
  set(MATH_LIB               m)
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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
  OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
  OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

  list(APPEND CMAKE_C_FLAGS "-std=gnu11")
  list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
endif()

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Xavi_SOURCE_DIR}/Readme.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Xavi is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Xavi-Cli "Xavi (Command Line)" XaviPP-Cli "XaviPP (Command Line)")
if (${BUILD_XAVI_GUI_WIN32})
  list(APPEND CPACK_PACKAGE_EXECUTABLES Xavi-Gui-Win32 "Xavi (Win32 GUI)")
endif()
if (${BUILD_XAVIPP_GUI_QT})
  list(APPEND CPACK_PACKAGE_EXECUTABLES XaviPP-Gui-Qt "Xavi (Qt GUI)")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR ${Xavi_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Xavi_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Xavi_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Xavi_SOURCE_DIR}/Readme.txt")

if(BUILD_XAVISTA)
  set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
endif()

if(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Xavi_SOURCE_DIR}/GPL-3.0.txt")
elseif(APPLE)
  set(CPACK_GENERATOR DragNDrop)
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(Xavi)
add_subdirectory(XaviPP)

# Allow other builds to include library headers.
include_directories(${Xavi_SOURCE_DIR})

# Command-line Programs
add_subdirectory(Xavi-Cli)
add_subdirectory(XaviPP-Cli)

# Tests
if(${BUILD_TESTS})
  add_subdirectory(Tests)
endif()

# GUI Programs
if(${BUILD_XAVI_GUI_COCOA})
  add_subdirectory(Xavi-Gui-Cocoa)
endif()
if(${BUILD_XAVIPP_GUI_QT})
  add_subdirectory(XaviPP-Gui-Qt)
endif()
if(${BUILD_XAVI_GUI_WIN32})
  add_subdirectory(Xavi-Gui-Win32)
endif()

##################################
# Preexisting File Installations #
##################################

install(
  FILES Readme.txt GPL-3.0.txt LGPL-3.0.txt
  DESTINATION ${DOC_INSTALL_PATH})

###############################################
# CPack Inclusion (Used for package creation) #
###############################################

include(CPack)
