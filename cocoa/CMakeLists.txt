# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2019, 2021 Vincent Damewood
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

set(SILIKO_INSTALL_APPDIR Applications CACHE STRING
	"Locaton to install application bundles (relative to install prefix)"
)

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(silikujo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikujoForMacOS MACOSX_BUNDLE)
target_sources(SilikujoForMacOS
	PRIVATE
		Delegate.h
		Delegate.m
		Main.m
		MainMenu.nib
		silikujo.icns
)
set_target_properties(SilikujoForMacOS PROPERTIES
	OUTPUT_NAME                         Silikujo
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikujoForMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikujo"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             ${COPYRIGHT_STATEMENT}
	MACOSX_BUNDLE_ICON_FILE             silikujo.icns
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
	RUNTIME_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/products"
)
target_compile_options(SilikujoForMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikujoForMacOS PRIVATE c_std_11)
target_link_libraries(SilikujoForMacOS
	PRIVATE
		Siliko::Siliko
		"-framework Cocoa"
)

install(TARGETS SilikujoForMacOS
BUNDLE
	DESTINATION  ${SILIKO_INSTALL_APPDIR}
	COMPONENT    Silikujo
)

install(CODE "include(BundleUtilities)")
install(CODE "fixup_bundle(\"$<TARGET_FILE:SilikujoForMacOS>\" \"\" \"$<TARGET_FILE_DIR:Siliko::Siliko>\")")
