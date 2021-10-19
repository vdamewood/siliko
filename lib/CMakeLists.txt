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

if(SILIKO_BUILD_FRAMEWORK)
	set(CMAKEDIR "${SILIKO_INSTALL_FRAMEWORKDIR}/Siliko.framework/Resources")
	set(RUNTIME_COMPONENT "Siliko")
	set(DEVELOPMENT_COMPONENT "Siliko")
else()
	set(CMAKEDIR "${SILIKO_INSTALL_CMAKEDIR}")
	set(RUNTIME_COMPONENT "Siliko_Runtime")
	set(DEVELOPMENT_COMPONENT "Siliko_Development")
endif()

if(SILIKO_BUILD_SHARED_LIBRARY)
	add_library(Siliko SHARED)
else()
	add_library(Siliko STATIC)
endif()
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
	target_link_libraries(Siliko PRIVATE m)
endif()

target_include_directories(Siliko
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
if (NOT SILIKO_BUILD_FRAMEWORK)
	target_include_directories(Siliko
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(Siliko PRIVATE c_std_11)
set_target_properties(Siliko PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 ${SILIKO_BUILD_FRAMEWORK}
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
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS Siliko
	EXPORT Siliko
RUNTIME
	DESTINATION           "${CMAKE_INSTALL_BINDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
ARCHIVE
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
LIBRARY
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
	NAMELINK_COMPONENT    "${DEVELOPMENT_COMPONENT}"
FRAMEWORK
	DESTINATION           "${SILIKO_INSTALL_FRAMEWORKDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
PUBLIC_HEADER
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}/Siliko"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(EXPORT Siliko
	NAMESPACE             Siliko::
	FILE                  SilikoConfig.cmake
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikoConfigVersion.cmake"
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Siliko"
	COMPONENT "${RUNTIME_COMPONENT}"
)

install(FILES
		"${PROJECT_SOURCE_DIR}/License.txt"
		"${PROJECT_SOURCE_DIR}/License.Gpl.txt"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Siliko"
	COMPONENT "${RUNTIME_COMPONENT}"
)
