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

set(TARGET SilikoCli)
set(SOURCES main.c)
set(DEPS ${LIB} ${LIB_DEPS})

message("-- Finding readline")
find_path(READLINE_INC NAMES readline/readline.h)
find_library(READLINE_LIB readline)

if(READLINE_LIB AND READLINE_INC)
	message("-- Finding readline -- found")
	list(APPEND DEPS ${READLINE_LIB})
	include_directories(${READLINE_INC})
	add_definitions(-DHAVE_READLINE=1)
else()
	message("-- Finding readline -- not found")
	add_definitions(-DHAVE_READLINE=0)
	list(APPEND SOURCES rl.c)
endif()

if (UNIX)
  add_definitions(-DUSE_UNIX=1)
endif()

add_executable(${TARGET} ${SOURCES})
set_target_properties(${TARGET} PROPERTIES
	OUTPUT_NAME ${ONAMECLI})
target_link_libraries(${TARGET} ${DEPS})

install(TARGETS ${TARGET}
  RUNTIME    DESTINATION ${INSTBIN})
