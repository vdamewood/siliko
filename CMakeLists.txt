# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood
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

cmake_minimum_required(VERSION 3.14)

project(Siliko
  LANGUAGES C
  VERSION   0.0.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(DESCRIPTION "Mathematical expression parser written in C")
set(VENDOR      "Vincent Damewood")
set(URL         "http://vdamewood.com/projects/siliko")
set(COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood.\
 Siliko library licensed under the terms of the GNU Lesser General Public\
 License, version 3 or later. Siliko interface programs licensed under the\
 terms of the GNU General Public License, version 3 or later.")
set(UPGRADE_GUID "9A29ACC2-5BDB-407C-AAFB-56897BABD10A")

#########################
# Configuration Options #
#########################

if(APPLE)
  option(FORCE_UNIX "Force Unix-style installation" Off)
endif()

option(BUILD_CLI "Build command-line program" On)
option(BUILD_GUI "Build GUI Program" On)

###############
# Build Style #
###############

if(APPLE)
  if(FORCE_UNIX)
    set(BUILD_UNIX On)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOS Off)
  else()
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOS On)
  endif()
elseif(UNIX)
    set(BUILD_UNIX On)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOS Off)
elseif(WIN32)
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS On)
    set(BUILD_MACOS Off)
else()
    set(BUILD_UNIX Off)
    set(BUILD_WINDOWS Off)
    set(BUILD_MACOS Off)
endif()

######################
# Build-Based Values #
######################

if(BUILD_MACOS)
  set(INSTBIN bin)
  set(INSTLIB err)
  set(INSTINC err)
  set(INSTSHR err)
  set(INSTDAT err)
  set(INSTDOC .)
  set(ONAMEDLIB Siliko)
  set(ONAMESLIB SilikoStatic) # This will be ignored.
  set(ONAMECLI  siliko)
  set(ONAMEGUI  "Siliko GUI")
elseif(BUILD_WINDOWS)
  set(INSTBIN .)
  set(INSTLIB .)
  set(INSTINC include)
  set(INSTSHR .)
  set(INSTDAT .)
  set(INSTDOC .)
  set(ONAMEDLIB siliko)
  set(ONAMESLIB siliko-static)
  set(ONAMECLI  siliko-cli)
  set(ONAMEGUI  siliko-gui)
else()
  include(GNUInstallDirs)
  set(INSTBIN ${CMAKE_INSTALL_BINDIR})
  set(INSTLIB ${CMAKE_INSTALL_LIBDIR})
  set(INSTINC ${CMAKE_INSTALL_INCLUDEDIR})
  set(INSTSHR ${CMAKE_INSTALL_DATADIR})
  set(INSTDAT "${INSTSHR}/${PROJECT_NAME}")
  set(INSTDOC ${CMAKE_INSTALL_DOCDIR})
  set(ONAMEDLIB siliko)
  set(ONAMESLIB siliko)
  set(ONAMECLI  siliko)
  set(ONAMEGUI  siliko-gui)
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(lib)
if(BUILD_CLI)
  add_subdirectory(command)
endif()
if(BUILD_GUI)
  if(BUILD_MACOS)
    add_subdirectory(cocoa)
  elseif(BUILD_UNIX)
    add_subdirectory(gtk3)
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
  FILES Readme.md Authors.txt LicenseStatement.txt License.txt License.Gpl.txt
  DESTINATION ${INSTDOC})

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "${VENDOR}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${DESCRIPTION}")

set(CPACK_STRIP_FILES On)
set(CPACK_VERBATIM_VARIABLES Off)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_STRIP_FILES Off)
set(CPACK_SOURCE_GENERATOR TXZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES "/\.git/" "/\.gitignore" "/[Bb]uild/")

if(BUILD_GUI)
  list(APPEND CPACK_PACKAGE_EXECUTABLES ${ONAMEGUI} ${PROJECT_NAME})
endif()

if(BUILD_MACOS)
  set(CPACK_GENERATOR DragNDrop)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-macOS")
  set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
  set(CPACK_DMG_FORMAT UDZO)
  set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK On)
elseif(BUILD_WINDOWS)
  if(MSVC)
    set(CPACK_GENERATOR ZIP WIX)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/cpack/WixLicense.txt")
    set(CPACK_WIX_UPGRADE_GUID ${UPGRADE_GUID})
  else()
    set(CPACK_GENERATOR ZIP)
  endif()
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-Windows")
else()
  set(CPACK_GENERATOR TXZ)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
endif()
set(BIN_PACKAGE "${CPACK_PACKAGE_FILE_NAME}" CACHE STRING "Filename for package" INTERNAL)
set(SRC_PACKAGE "${PROJECT_NAME}-${PROJECT_VERSION}" CACHE STRING "Filename for source package" INTERNAL)
include(CPack)
