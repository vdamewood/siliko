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

set(EXENAME Siliko-Gui-Win32)
set(HDRS EvalWindow.h About.h Id.h)
set(SRCS Siliko-Gui-Win32.c EvalWindow.c EvalWindow.rc About.c About.rc)
set(LIBS ${LIB})

message("-- Finding User32.dll")
find_library(USER32 User32)
if (NOT ${USER32} STREQUAL "USER32-NOTFOUND")
  message("-- Finding User32.dll - found")
  list(APPEND LIBS ${USER32})
  set(USER32_FOUND Yes)
else()
  message("-- Finding User32.dll - not found")
  set(USER32_FOUND No)
endif()

if (NOT USER32_FOUND)
  message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_executable(${EXENAME} WIN32 ${HDRS} ${SRCS})
target_link_libraries(${EXENAME} ${LIBS})

install(TARGETS ${EXENAME}
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})
