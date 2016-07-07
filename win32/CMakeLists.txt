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

set(TARGET SilikoGui)
set(HEADERS EvalWindow.h About.h Id.h)
set(SOURCES WinMain.c EvalWindow.c EvalWindow.rc About.c About.rc)
set(DEPS ${LIB})

message("-- Finding User32.dll")
find_library(USER32 User32)
if (NOT ${USER32} STREQUAL "USER32-NOTFOUND")
  message("-- Finding User32.dll - found")
  list(APPEND DEPS ${USER32})
  set(USER32_FOUND Yes)
else()
  message("-- Finding User32.dll - not found")
  set(USER32_FOUND No)
endif()

if (NOT USER32_FOUND)
  message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_executable(${TARGET} WIN32 ${HEADERS} ${SOURCES})
set_target_properties(${TARGET} PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_link_libraries(${TARGET} ${DEPS})

install(TARGETS ${TARGET}
  RUNTIME DESTINATION ${INSTBIN})
