# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015 Vincent Damewood
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

set(EXENAME Xavi-Gui-Gtk)
set(HDRS EvalWindow.h)
set(SRCS Xavi-Gui-Gtk.c EvalWindow.c)
set(LIBS ${CLIB} ${GTK2_LIBRARIES})

include_directories(${GTK2_INCLUDE_DIRS})
add_definitions("-DSHARE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
add_executable(${EXENAME} ${SRCS} ${HDRS})
target_link_libraries(${EXENAME} ${LIBS})

install(TARGETS ${EXENAME}
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH})
