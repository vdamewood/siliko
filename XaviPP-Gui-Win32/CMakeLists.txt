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

set(EXENAME XaviPP-Gui-Win32)
set(HDRS EvalWindow.hpp About.hpp Id.h)
set(SRCS XaviPP-Gui-Win32.cpp EvalWindow.cpp EvalWindow.rc About.cpp About.rc)
set(LIBS ${CXXLIB})

ADD_DEFINITIONS(-D_AFXDLL -D_WIN32_WINNT=NTDDI_WIN7)
SET(CMAKE_MFC_FLAG 2) 

add_executable(${EXENAME} WIN32 ${HDRS} ${SRCS})
target_link_libraries(${EXENAME} ${LIBS})

install(TARGETS ${EXENAME}
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})
