# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood
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

add_executable(SilikoCli)
target_sources(SilikoCli PRIVATE main.c)
target_link_libraries(SilikoCli PRIVATE Siliko::Core)
target_compile_features(SilikoCli PRIVATE c_std_11)
set_target_properties(SilikoCli PROPERTIES
	OUTPUT_NAME ${ONAMECLI})

if (UNIX)
	target_compile_definitions(SilikoCli PRIVATE USE_UNIX=1)
endif()

if(Readline_FOUND)
	target_link_libraries(SilikoCli PRIVATE Readline::Readline)
	target_compile_definitions(SilikoCli PRIVATE HAVE_READLINE=1)
else()
	target_sources(SilikoCli PRIVATE rl.c)
	target_compile_definitions(SilikoCli PRIVATE HAVE_READLINE=0)
endif()

if(WIN32)
	add_custom_command(
		TARGET SilikoCli POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:Siliko::Core>
		$<$<CONFIG:Debug>:$<TARGET_PDB_FILE:Siliko::Core>>
		${CMAKE_CURRENT_BINARY_DIR})
endif()

install(TARGETS SilikoCli
	RUNTIME    DESTINATION ${INSTBIN})
