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

add_library(Siliko)
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
		SilikoConfig.cmake
)
add_library(Siliko::Siliko ALIAS Siliko)
target_include_directories(Siliko
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_BINARY_DIR}/include"
)
target_compile_features(Siliko PRIVATE c_std_11)
set_target_properties(Siliko PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	RESOURCE                  SilikoConfig.cmake
	FRAMEWORK                 TRUE
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)

include(GenerateExportHeader)
generate_export_header(Siliko
	EXPORT_FILE_NAME include/Siliko/Api.h
	EXPORT_MACRO_NAME SILIKO_API
)

find_library(LIBM m)
if(LIBM)
	target_link_libraries(Siliko PUBLIC m)
endif()

install(TARGETS Siliko
	RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
	ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
	FRAMEWORK     DESTINATION .
	RESOURCE      DESTINATION ${CMAKE_INSTALL_DATADIR}/Siliko
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Siliko
)
