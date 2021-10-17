# CMakeLists.txt: Build Script
# Copyright 2012-2021 Vincent Damewood
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

project(Siliko
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/siliko"
	LANGUAGES     C
)


#########################
# Configuration Options #
#########################

include(GNUInstallDirs)

option(SILIKO_BUILD_SHARED_LIBRARY  "Build Siliko as a shared library (build as a static library is off)" On)
option(SILIKO_BUILD_CLI             "Build Silikilo, a command-line interface for Siliko" On)
option(SILIKO_BUILD_GUI             "Build Silikujo, a GUI interface for Siliko" On)
option(SILIKO_PACKAGE               "Add 'package' and 'package_source' build targets for packages" On)
option(SILIKO_PACKAGE_ZIP           "Package as zip file (ignored if SILIKO_PACKAGE is off)" On)
option(SILIKO_PACKAGE_7ZIP          "Package as 7zip file (ignored if SILIKO_PACKAGE is off)" On)
option(SILIKO_PACKAGE_TXZ           "Package as XZ-compressed tar file (ignored if SILIKO_PACKAGE is off)" On)

set(SILIKO_PACKAGE_VENDOR  "Unknown Vendor"  CACHE STRING
	"The name of the vendor when generating packages (ignored if SILIKO_PACKAGE is off)"
)

if(APPLE)
	option(SILIKO_PACKAGE_DMG       "Package as DMG file (ignored if SILIKO_PACKAGE is off)" On)

	set(SILIKO_INSTALL_APPDIR Applications  CACHE STRING
		"Locaton to install application bundles"
	)
	set(SILIKO_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKO_BUILD_FRAMEWORK is off)"
	)
else()
	set(SILIKO_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Siliko
		CACHE STRING "Locaton to install CMake files"
	)
endif()

if(WIN32)
	option(SILIKO_PACKAGE_MSI       "Package as MSI Installer  (Ignored if SILIKO_PACKAGE is false)" On)

	set(SILIKO_MSI_GUID  "00000000-0000-0000-0000-000000000000"  CACHE STRING
		"The Upgrade GUID used for MSI Installers (Ignored if SILIKO_PACKAGE or SILIKO_PACKAGE_MSI is false)"
	)
endif()


###########
# Targets #
###########

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(lib)
if(SILIKO_BUILD_CLI)
	add_subdirectory(command)
endif()
if(SILIKO_BUILD_GUI)
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

install(FILES Readme.md License.txt License.Gpl.txt
	DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/Siliko
	COMPONENT Siliko_Runtime
)


#############################
# Package-creation Settings #
#############################

if (SILIKO_PACKAGE)
	set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
	set(CPACK_PACKAGE_VENDOR "${SILIKO_PACKAGE_VENDOR}")
	set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
	set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")

	set(CPACK_STRIP_FILES On)
	set(CPACK_SOURCE_STRIP_FILES Off)
	set(CPACK_VERBATIM_VARIABLES Off)
	set(CPACK_SOURCE_IGNORE_FILES "/\.git/" "/\.gitignore" "/[Bb]uild/")

	if(SILIKO_PACKAGE_7ZIP)
		list(APPEND CPACK_GENERATOR 7Z)
		list(APPEND CPACK_SOURCE_GENERATOR 7Z)
	endif()
	if(SILIKO_PACKAGE_TXZ)
		list(APPEND CPACK_GENERATOR TXZ)
		list(APPEND CPACK_SOURCE_GENERATOR TXZ)
	endif()
	if(SILIKO_PACKAGE_ZIP)
		list(APPEND CPACK_GENERATOR ZIP)
		list(APPEND CPACK_SOURCE_GENERATOR ZIP)
	endif()

	set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
	if(APPLE)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-macOS")
	elseif(WIN32)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-Windows")
	else()
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
	endif()

	if(SILIKO_BUILD_GUI)
		list(APPEND CPACK_PACKAGE_EXECUTABLES Silikujo Silikujo)
	endif()

	if(SILIKO_PACKAGE_DMG)
		list(APPEND CPACK_GENERATOR DragNDrop)
		set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
		set(CPACK_DMG_FORMAT UDZO)
		set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK On)
	endif()

	if(SILIKO_PACKAGE_MSI)
		list(APPEND CPACK_GENERATOR WIX)
		set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/cpack/WixLicense.txt")
		set(CPACK_WIX_UPGRADE_GUID "${SILIKO_MSI_GUID}")
	endif()

	include(CPack)
endif()
