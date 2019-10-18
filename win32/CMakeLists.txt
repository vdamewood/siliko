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

add_executable(SilikujoForWindows WIN32)
target_Sources(SilikujoForWindows
	PRIVATE
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
set_target_properties(SilikujoForWindows PROPERTIES
	OUTPUT_NAME Silikujo)
target_compile_features(SilikujoForWindows PRIVATE c_std_11)
target_link_libraries(SilikujoForWindows PRIVATE Siliko::Core)

add_custom_command(
	TARGET SilikujoForWindows POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Siliko::Core>
	$<$<CONFIG:Debug>:$<TARGET_PDB_FILE:Siliko::Core>>
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS SilikujoForWindows
	RUNTIME DESTINATION ${INSTBIN})
