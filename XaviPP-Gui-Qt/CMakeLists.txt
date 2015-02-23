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

set(EXENAME XaviPP-Gui-Qt)
set(MHDRS EvalWindow.hpp)
set(HDRS ${MHDRS} QStringSource.hpp)
set(SRCS XaviPP-Gui-Qt.cpp EvalWindow.cpp QStringSource.cpp)
set(QUIS EvalWindow.ui)
set(LIBS ${CXXLIB} ${Qt5Widgets_LIBRARIES})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS})
qt5_wrap_ui(QUISRCS ${QUIS})
qt5_wrap_cpp(MSRCS ${XaviPP_Gui_Qt_MOC_HEADERS})

add_executable(${EXENAME} WIN32 MACOSX_BUNDLE ${SRCS} ${QUISRCS} ${MSRCS})
target_link_libraries(${EXENAME} ${LIBS})
install(TARGETS ${EXENAME}
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})

if(APPLE)
    set(PREPARE_BUNDLE ON)
    set(BUNDLE_EXENAME "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app")
    set(BUNDLE_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app/Contents/PlugIns/")
    install(CODE
      "FILE(
        INSTALL \"${Xavi_SOURCE_DIR}/XaviPP-Gui-Qt/qt.conf\"
        DESTINATION \"\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app/Contents/Resources\"
       )"
    )
elseif(WIN32)
    set(PREPARE_BUNDLE ON)
    set(BUNDLE_EXENAME "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.exe")
    set(BUNDLE_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}")
else()
    set(PREPARE_BUNDLE OFF)
endif()

if(PREPARE_BUNDLE)
  foreach(plugin_target ${Qt5Gui_PLUGINS})
    get_target_property(plugin ${plugin_target} LOCATION)
    string(FIND "${plugin}" "plugins/" idx REVERSE)
    math(EXPR idx "${idx} + 8")
    string(SUBSTRING "${plugin}" "${idx}" -1 short)
    string(FIND ${short} "/" idxs REVERSE)
    string(SUBSTRING ${short} 0 ${idxs} directory)
    install(CODE "FILE(INSTALL ${plugin} DESTINATION \"${BUNDLE_PLUGIN_DIR}/${directory}\")")
    list(APPEND XaviPP_Gui_Qt_PLUGINS "${BUNDLE_PLUGIN_DIR}/${short}")
  endforeach()

  install(CODE "include(BundleUtilities)")
  install(CODE "fixup_bundle(\"${BUNDLE_EXENAME}\" \"${XaviPP_Gui_Qt_PLUGINS}\" \"${XaviPP_BINARY_DIR}\")")
endif()
