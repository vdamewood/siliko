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

add_library(SilikoCore SHARED
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
add_library(Siliko::Core ALIAS SilikoCore)
target_include_directories(SilikoCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(SilikoCore PROPERTIES
		PUBLIC_HEADER
			W32Dll.h
			Value.h
			FunctionCaller.h
			SyntaxTree.h
			DataSource.h
			StringSource.h
			Token.h
			Lexer.h
			InfixParser.h
		RESOURCE
			SilikoConfig.cmake
		OUTPUT_NAME       ${ONAMEDLIB}
		FRAMEWORK         ${BUILD_MACOS}
		FRAMEWORK_VERSION ${PROJECT_VERSION}
		VERSION           ${PROJECT_VERSION}
		SOVERSION         ${PROJECT_VERSION_MAJOR})

find_library(LIBM m)
if(LIBM)
	target_link_libraries(SilikoCore PUBLIC m)
endif()

install(TARGETS SilikoCore
	RUNTIME       DESTINATION ${INSTBIN}
	ARCHIVE       DESTINATION ${INSTLIB}
	LIBRARY       DESTINATION ${INSTLIB}
	FRAMEWORK     DESTINATION .
	RESOURCE      DESTINATION ${INSTDAT}
	PUBLIC_HEADER DESTINATION ${INSTINC}/Siliko)

if(BUILD_UNIX)
	configure_file(Siliko.pc.in Siliko.pc @ONLY)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Siliko.pc" DESTINATION "${INSTLIB}/pkgconfig")
endif()
