# CMakeLists.txt: Packaging settings
# Copyright 2012-2022 Vincent Damewood
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "${SILIKO_PACKAGE_VENDOR}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")

set(CPACK_STRIP_FILES On)
set(CPACK_SOURCE_STRIP_FILES Off)
set(CPACK_VERBATIM_VARIABLES Off)
set(CPACK_SOURCE_IGNORE_FILES
    "/\.git/"
    "/\.gitignore"
    "/*.code-workspace"
    "/[Bb]uild*/"
)
if(SILIKO_BUILD_GUI)
    list(APPEND CPACK_PACKAGE_EXECUTABLES Silikujo Silikujo)
endif()

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
if(APPLE)
    set(OS "macOS")
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64" AND CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
        set(ARCH "Universal2")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(ARCH "AppleSilicon")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(ARCH "Intel")
    else()
        set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
elseif(WIN32)
    set(OS "Windows")
    set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
else()
    set(OS "${CMAKE_SYSTEM_NAME}")
    set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${OS}-${ARCH}")

if(SILIKO_PACKAGE_7ZIP)
    list(APPEND CPACK_GENERATOR 7Z)
    list(APPEND CPACK_SOURCE_GENERATOR 7Z)
    set(CPACK_7IP_COMPONENT_INSTALL On)
endif()

if(SILIKO_PACKAGE_TXZ)
    list(APPEND CPACK_GENERATOR TXZ)
    list(APPEND CPACK_SOURCE_GENERATOR TXZ)
    set(CPACK_TXZ_COMPONENT_INSTALL On)
endif()

if(SILIKO_PACKAGE_ZIP)
    list(APPEND CPACK_GENERATOR ZIP)
    list(APPEND CPACK_SOURCE_GENERATOR ZIP)
    set(CPACK_ZIP_COMPONENT_INSTALL On)
endif()

if(SILIKO_PACKAGE_DMG)
    list(APPEND CPACK_GENERATOR DragNDrop)
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_DMG_FORMAT UDZO)
    set(CPACK_DMG_DISABLE_APPLICATIONS_SYMLINK On)
endif()

if(SILIKO_PACKAGE_MSI)
    list(APPEND CPACK_GENERATOR WIX)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/WixLicense.txt")
    set(CPACK_WIX_UPGRADE_GUID "${SILIKO_MSI_GUID}")
endif()

include(CPack)
