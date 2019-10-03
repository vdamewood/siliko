# FindReadline.cmake: Find Readline
# Copyright 2019 Vincent Damewood
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

find_library(READLINE_LIB readline)
find_path(READLINE_INC NAMES readline/readline.h)
if(READLINE_LIB AND READLINE_INC)
	add_library(Readline SHARED IMPORTED GLOBAL)
	add_library(Readline::Readline ALIAS Readline)
	set_property(TARGET Readline PROPERTY IMPORTED_LOCATION "${READLINE_LIB}")
	target_include_directories(Readline INTERFACE "${READLINE_INC}")
	set(Readline_FOUND 1)
else()
	set(Readline_FOUND 0)
endif()

mark_as_advanced(
	READLINE_INC
	READLINE_LIB
)
