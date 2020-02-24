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

cmake_minimum_required(VERSION 3.14)


##########################
# Project Identification #
##########################

project(Siliko
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)

set(VENDOR              "Vincent Damewood")
set(UPGRADE_GUID        "9A29ACC2-5BDB-407C-AAFB-56897BABD10A")
set(COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood.\
 Siliko library licensed under the terms of the GNU Lesser General Public\
 License, version 3 or later. Siliko interface programs licensed under the\
 terms of the GNU General Public License, version 3 or later.")


#########################
# Configuration Options #
#########################

option(BUILD_CLI "Build command-line program" On)
option(BUILD_GUI "Build GUI Program" On)
option(BUILD_SHARED_LIBS "Build as a shared library" On)

if(APPLE)
	option(PACK_DND "Package as Drag-and-Drop disk image" On)
endif()

if(WIN32)
	option(PACK_MSI "Package as MSI Installer" On)
endif()


######################
# Build-Based Values #
######################

if(APPLE)
	set(INSTBIN bin)
	set(INSTLIB err)
	set(INSTINC err)
	set(INSTSHR err)
	set(INSTDAT err)
	set(INSTDOC .)
elseif(WIN32)
	set(INSTBIN .)
	set(INSTLIB .)
	set(INSTINC include)
	set(INSTSHR .)
	set(INSTDAT .)
	set(INSTDOC .)
else()
	include(GNUInstallDirs)
	set(INSTBIN ${CMAKE_INSTALL_BINDIR})
	set(INSTLIB ${CMAKE_INSTALL_LIBDIR})
	set(INSTINC ${CMAKE_INSTALL_INCLUDEDIR})
	set(INSTSHR ${CMAKE_INSTALL_DATADIR})
	set(INSTDAT "${INSTSHR}/${PROJECT_NAME}")
	set(INSTDOC ${CMAKE_INSTALL_DOCDIR})
endif()


###########
# Targets #
###########

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(lib)
if(BUILD_CLI)
	add_subdirectory(command)
endif()
if(BUILD_GUI)
	if(APPLE)
		add_subdirectory(cocoa)
	elseif(UNIX)
		add_subdirectory(gtk3)
	elseif(WIN32)
		add_subdirectory(win32)
	else()
		message(FATAL_ERROR "A GUI program is not available for this system.")
	endif()
endif()

install(
	FILES
		Readme.md
		License.txt
		License.Gpl.txt
	DESTINATION ${INSTDOC}
)


#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "${VENDOR}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")

set(CPACK_STRIP_FILES On)
set(CPACK_GENERATOR 7Z TXZ ZIP)
set(CPACK_VERBATIM_VARIABLES Off)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_STRIP_FILES Off)
set(CPACK_SOURCE_GENERATOR 7Z TXZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES "/\.git/" "/\.gitignore" "/[Bb]uild/")

if(APPLE)
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-macOS")
elseif(WIN32)
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-Windows")
else()
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
endif()

if(BUILD_GUI)
	list(APPEND CPACK_PACKAGE_EXECUTABLES Silikujo Silikujo)
endif()

if(PACK_DND)
	list(APPEND CPACK_GENERATOR DragNDrop)
	set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
	set(CPACK_DMG_FORMAT UDZO)
	set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK On)
endif()

if(PACK_MSI)
	list(APPEND CPACK_GENERATOR WIX)
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/cpack/WixLicense.txt")
	set(CPACK_WIX_UPGRADE_GUID ${UPGRADE_GUID})
endif()

include(CPack)
