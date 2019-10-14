# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

add_executable(SilikoGui WIN32
	EvalWindow.h
	About.h
	Id.h
	WinMain.c
	EvalWindow.c
	EvalWindow.rc
	About.c
	About.rc
	Icon.rc
)
set_target_properties(SilikoGui PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_compile_features(SilikoGui PRIVATE c_std_11)
target_link_libraries(SilikoGui PRIVATE Siliko::Core)

add_custom_command(
	TARGET SilikoGui POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Siliko::Core>
	$<$<CONFIG:Debug>:$<TARGET_PDB_FILE:Siliko::Core>>
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS SilikoGui
	RUNTIME DESTINATION ${INSTBIN})
