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

set(EXENAME Xavi-Gui-Qt)
set(MHDRS EvalWindow.hpp)
set(HDRS ${MHDRS} QStringSource.hpp)
set(SRCS XaviPP-Gui-Qt.cpp EvalWindow.cpp QStringSource.cpp)
set(QUIS EvalWindow.ui)
set(LIBS ${CLIB} ${Qt5Widgets_LIBRARIES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS})
qt5_wrap_ui(QUISRCS ${QUIS})
qt5_wrap_cpp(MSRCS ${MHDRS})

if(APPLE)
  set(RESOURCES qt.conf)
  set_source_files_properties(qt.conf PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
else()
   set(RESOURCES "")
endif()

add_executable(${EXENAME} WIN32 MACOSX_BUNDLE ${SRCS} ${QUISRCS} ${MSRCS} ${RESOURCES})
target_link_libraries(${EXENAME} ${LIBS})
install(TARGETS ${EXENAME}
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})

if(APPLE)
    set(PREPARE_BUNDLE ON)
    set(BUNDLE_EXENAME "\${CMAKE_INSTALL_PREFIX}/${EXENAME}.app")
    set(BUNDLE_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}/${EXENAME}.app/Contents/PlugIns/")
elseif(WIN32)
    set(PREPARE_BUNDLE ON)
    set(BUNDLE_EXENAME "\${CMAKE_INSTALL_PREFIX}/${EXENAME}.exe")
    set(BUNDLE_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}")
else()
    set(PREPARE_BUNDLE OFF)
endif()

if(PREPARE_BUNDLE)
  foreach(PLUGIN_TARGET ${Qt5Gui_PLUGINS})
    get_target_property(PLUGIN_ABSOLUTE_PATH ${PLUGIN_TARGET} LOCATION)
    string(FIND "${PLUGIN_ABSOLUTE_PATH}" "plugins/" INDEX REVERSE)
    math(EXPR INDEX "${INDEX} + 8")
    string(SUBSTRING ${PLUGIN_ABSOLUTE_PATH} ${INDEX} -1 PLUGIN_RELATIVE_PATH)
    string(FIND ${PLUGIN_RELATIVE_PATH} "/" INDEX REVERSE)
    string(SUBSTRING ${PLUGIN_RELATIVE_PATH} 0 ${INDEX} PLUGIN_RELATIVE_DIR)

    install(CODE "FILE(INSTALL ${PLUGIN_ABSOLUTE_PATH} DESTINATION \"${BUNDLE_PLUGIN_DIR}/${PLUGIN_RELATIVE_DIR}\")")
    list(APPEND PLUGINS "${BUNDLE_PLUGIN_DIR}/${PLUGIN_RELATIVE_PATH}")
  endforeach()

  install(CODE "include(BundleUtilities)")
  install(CODE "fixup_bundle(\"${BUNDLE_EXENAME}\" \"${PLUGINS}\" ${CXXLIB_BINARY_DIR})")
endif()
