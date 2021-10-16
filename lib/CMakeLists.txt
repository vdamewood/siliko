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

if(APPLE)
	set(SILIKO_INSTALL_FRAMEWORKDIR Frameworks
		CACHE STRING "Locaton to install framework bundles (relative to install prefix)"
	)
	set(SILIKO_INSTALL_CMAKEDIR "${SILIKO_INSTALL_FRAMEWORKDIR}/Siliko.framework/Resources")
else()
	set(SILIKO_INSTALL_FRAMEWORKDIR Frameworks)
	set(SILIKO_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Siliko
		CACHE STRING "Locaton to install CMake package files (relative to install prefix)"
	)
endif()

add_library(Siliko)
add_library(Siliko::Siliko ALIAS Siliko)

set(PUBLIC_HEADERS
	include/Siliko/Value.h
	include/Siliko/FunctionCaller.h
	include/Siliko/SyntaxTree.h
	include/Siliko/DataSource.h
	include/Siliko/StringSource.h
	include/Siliko/Token.h
	include/Siliko/Lexer.h
	include/Siliko/InfixParser.h
	${CMAKE_CURRENT_BINARY_DIR}/include/Siliko/Api.h
)
target_sources(Siliko
	PRIVATE
		${PUBLIC_HEADERS}
		src/Functions.h
		src/Hash.h
		src/Value.c
		src/Hash.c
		src/Functions.c
		src/FunctionCaller.c
		src/SyntaxTree.c
		src/DataSource.c
		src/StringSource.c
		src/Lexer.c
		src/InfixParser.c
)

find_library(LIBM m)
if(LIBM)
	target_link_libraries(Siliko PUBLIC m)
endif()

target_include_directories(Siliko
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)
if (NOT APPLE)
	target_include_directories(Siliko
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(Siliko PRIVATE c_std_11)
set_target_properties(Siliko PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 TRUE
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)

include(GenerateExportHeader)
generate_export_header(Siliko
	EXPORT_FILE_NAME include/Siliko/Api.h
	EXPORT_MACRO_NAME SILIKO_API
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikoConfigVersion.cmake
	VERSION ${Siliko_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS Siliko
	EXPORT Siliko
RUNTIME
	DESTINATION           ${CMAKE_INSTALL_BINDIR}
	COMPONENT             Siliko_Runtime
ARCHIVE
	DESTINATION           ${CMAKE_INSTALL_LIBDIR}
	COMPONENT             Siliko_Development
LIBRARY
	DESTINATION           ${CMAKE_INSTALL_LIBDIR}
	COMPONENT             Siliko_Runtime
	NAMELINK_COMPONENT    Siliko_Development
FRAMEWORK
	DESTINATION           ${SILIKO_INSTALL_FRAMEWORKDIR}
	COMPONENT             Siliko_Development
PUBLIC_HEADER
	DESTINATION           ${CMAKE_INSTALL_INCLUDEDIR}/Siliko
	COMPONENT             Siliko_Development
)
install(EXPORT Siliko
	NAMESPACE             Siliko::
	FILE                  SilikoConfig.cmake
	DESTINATION           ${SILIKO_INSTALL_CMAKEDIR}
	COMPONENT             Siliko_Development
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SilikoConfigVersion.cmake
	DESTINATION           ${SILIKO_INSTALL_CMAKEDIR}
	COMPONENT             Siliko_Development
)
