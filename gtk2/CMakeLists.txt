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

set(EXENAME Siliko-Gui)
set(HDRS EvalWindow.h)
set(SRCS Main.c EvalWindow.c)

set(LIBS ${LIB})
message("-- Finding GTK+ 2")
find_package(GTK2 QUIET COMPONENTS gtk)
if(GTK2_FOUND)
  message("-- Finding GTK+ 2 - found")
  list(APPEND LIBS ${GTK2_LIBRARIES})
  include_directories(${GTK2_INCLUDE_DIRS})
else()
  message("-- Finding GTK+ 2 - not found")
endif()

if(NOT GTK2_FOUND)
  message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_definitions("-DSHARE_PATH=\"${CMAKE_INSTALL_PREFIX}/${SHARE_INSTALL_PATH}\"")
add_executable(${EXENAME} ${SRCS} ${HDRS})
target_link_libraries(${EXENAME} ${LIBS})

install(TARGETS ${EXENAME}
  RUNTIME    DESTINATION ${RUNTIME_INSTALL_PATH})

install(
  FILES EvalWindow.glade
  DESTINATION ${SHARE_INSTALL_PATH})
