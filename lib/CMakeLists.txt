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

add_library(Siliko SHARED)
target_sources(Siliko
	PRIVATE
		W32Dll.h
		Value.h
		FunctionCaller.h
		SyntaxTree.h
		DataSource.h
		StringSource.h
		Token.h
		Lexer.h
		InfixParser.h
		Functions.h
		Hash.h
		Value.c
		Hash.c
		Functions.c
		FunctionCaller.c
		SyntaxTree.c
		DataSource.c
		StringSource.c
		Lexer.c
		InfixParser.c
		SilikoConfig.cmake
)
add_library(Siliko::Siliko ALIAS Siliko)
target_include_directories(Siliko PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Siliko PRIVATE c_std_11)

set(PUBLIC_HEADERS
	W32Dll.h
	Value.h
	FunctionCaller.h
	SyntaxTree.h
	DataSource.h
	StringSource.h
	Token.h
	Lexer.h
	InfixParser.h
)
set_target_properties(Siliko PROPERTIES
		PUBLIC_HEADER     "${PUBLIC_HEADERS}"
		RESOURCE
			SilikoConfig.cmake
		FRAMEWORK         TRUE
		FRAMEWORK_VERSION ${PROJECT_VERSION}
		VERSION           ${PROJECT_VERSION}
		SOVERSION         ${PROJECT_VERSION_MAJOR})

find_library(LIBM m)
if(LIBM)
	target_link_libraries(Siliko PUBLIC m)
endif()

install(TARGETS Siliko
	RUNTIME       DESTINATION ${INSTBIN}
	ARCHIVE       DESTINATION ${INSTLIB}
	LIBRARY       DESTINATION ${INSTLIB}
	FRAMEWORK     DESTINATION .
	RESOURCE      DESTINATION ${INSTDAT}
	PUBLIC_HEADER DESTINATION ${INSTINC}/Siliko)

if(UNIX AND NOT APPLE)
	configure_file(Siliko.pc.in Siliko.pc @ONLY)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Siliko.pc" DESTINATION "${INSTLIB}/pkgconfig")
endif()
