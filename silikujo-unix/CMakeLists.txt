# CMakeLists.txt: Build Script
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(Gtk REQUIRED gtk4 IMPORTED_TARGET)

find_program(GRESOURCE glib-compile-resources)
if(NOT GRESOURCE)
	message(FATAL_ERROR "Can't find glib-compile-resources.")
endif()

add_custom_command(
	OUTPUT gresource.h
	COMMAND "${GRESOURCE}"
		--generate-header
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.h
		gresource.xml
	DEPENDS gresource.xml EvalWindow.ui
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
	OUTPUT gresource.c
	COMMAND "${GRESOURCE}"
		--generate-source
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.c
		gresource.xml
	DEPENDS gresource.xml EvalWindow.ui
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SilikujoForUnix)
target_sources(SilikujoForUnix
	PRIVATE
		Main.c
		EvalWindow.c
		gresource.c
		EvalWindow.h
		gresource.h
)
set_target_properties(SilikujoForUnix PROPERTIES
	OUTPUT_NAME               Silikujo
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)
target_compile_features(SilikujoForUnix PRIVATE c_std_11)
target_link_libraries(SilikujoForUnix PRIVATE Siliko::Siliko PkgConfig::Gtk)

install(TARGETS SilikujoForUnix
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikujo
)

install(FILES silikujo.desktop
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/applications"
	COMPONENT    Silikujo
)

install(FILES siliko.png
	DESTINATION  "${CMAKE_INSTALL_DATADIR}/icons/hicolor/48x48/apps"
	COMPONENT    Silikujo
)
