# XaviPP-Gui-Qt

set(XaviPP_Gui_Qt_HEADERS     EvalWindow.hpp QStringSource.hpp)
set(XaviPP_Gui_Qt_MOC_HEADERS EvalWindow.hpp)
set(XaviPP_Gui_Qt_SOURCES     XaviPP-Gui-Qt.cpp EvalWindow.cpp QStringSource.cpp)
set(XaviPP_Gui_Qt_UIS         EvalWindow.ui)
set(XaviPP_Gui_Qt_LIBRARIES   XaviPP ${Qt5Widgets_LIBRARIES})
set(XaviPP_Gui_Qt_PLUGINS)

message(" Linking XaviPP with: " "${XaviPP_Gui_Qt_LIBRARIES}")

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Xavi_SOURCE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_ui(XaviPP_Gui_Qt_UI_SOURCES ${XaviPP_Gui_Qt_UIS})
qt5_wrap_cpp(XaviPP_Gui_Qt_MOC_SOURCES ${XaviPP_Gui_Qt_MOC_HEADERS})

add_executable(XaviPP-Gui-Qt
  WIN32 MACOSX_BUNDLE
  ${XaviPP_Gui_Qt_SOURCES}
  ${XaviPP_Gui_Qt_UI_SOURCES}
  ${XaviPP_Gui_Qt_MOC_SOURCES})
target_link_libraries(XaviPP-Gui-Qt
  ${XaviPP_Gui_Qt_LIBRARIES})

install(TARGETS XaviPP-Gui-Qt
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH}
)

if(APPLE)
    set(PREPARE_BUNDLE ON)
    set(XaviPP_Gui_Qt_EXECUTABLE "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app")
    set(XaviPP_Gui_Qt_QT_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app/Contents/PlugIns/")
    install(CODE
      "FILE(
        INSTALL \"${Xavi_SOURCE_DIR}/XaviPP-Gui-Qt/qt.conf\"
        DESTINATION \"\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.app/Contents/Resources\"
       )"
    )
elseif(WIN32)
    set(PREPARE_BUNDLE ON)
    set(XaviPP_Gui_Qt_EXECUTABLE "\${CMAKE_INSTALL_PREFIX}/XaviPP-Gui-Qt.exe")
    set(XaviPP_Gui_Qt_QT_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}")
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
    install(CODE "FILE(INSTALL ${plugin} DESTINATION \"${XaviPP_Gui_Qt_QT_PLUGIN_DIR}/${directory}\")")
    list(APPEND XaviPP_Gui_Qt_PLUGINS "${XaviPP_Gui_Qt_QT_PLUGIN_DIR}/${short}")
  endforeach()

  install(CODE "include(BundleUtilities)")
  install(CODE "fixup_bundle(\"${XaviPP_Gui_Qt_EXECUTABLE}\" \"${XaviPP_Gui_Qt_PLUGINS}\" \"${XaviPP_BINARY_DIR}\")")
endif()
