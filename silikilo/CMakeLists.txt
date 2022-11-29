# CMakeLists.txt: Build Script
# Copyright 2012-2021 Vincent Damewood
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

find_package(Readline)

add_executable(Silikilo)
target_sources(Silikilo
	PRIVATE
		main.c
)
set_target_properties(Silikilo PROPERTIES
	OUTPUT_NAME               silikilo
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)
target_compile_features(Silikilo PRIVATE c_std_11)
target_link_libraries(Silikilo PRIVATE Siliko::Siliko)

if (UNIX)
	target_compile_definitions(Silikilo PRIVATE USE_UNIX=1)
endif()

if(Readline_FOUND)
	target_link_libraries(Silikilo PRIVATE Readline::Readline)
	target_compile_definitions(Silikilo PRIVATE HAVE_READLINE=1)
else()
	target_sources(Silikilo PRIVATE rl.c)
	target_compile_definitions(Silikilo PRIVATE HAVE_READLINE=0)
endif()

if(APPLE AND SILIKO_BUILD_SHARED_LIBRARY)
	if (SILIKO_BUILD_FRAMEWORK)
		set_target_properties(Silikilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../Frameworks"
		)
	else()
		set_target_properties(Silikilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../lib"
		)
	endif()
endif()

install(TARGETS Silikilo
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikilo
)
