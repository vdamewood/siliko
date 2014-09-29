# Xavista

set(Xavista_HEADERS     ${Library_SOURCE_DIR}/Xavi.h)
set(Xavista_MOC_HEADERS XavistaWindow.h)
set(Xavista_SOURCES     Xavista.cpp XavistaWindow.cpp)
set(Xavista_UIS         XavistaWindow.ui)
set(Xavista_LIBRARIES   Xavi ${Qt5Widgets_LIBRARIES})
set(Xavista_PLUGINS)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Xavi_SOURCE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_ui(Xavista_UI_SOURCES ${Xavista_UIS})
qt5_wrap_cpp(Xavista_MOC_SOURCES ${Xavista_MOC_HEADERS})

add_executable(Xavista
  WIN32 MACOSX_BUNDLE
  ${Xavista_SOURCES}
  ${Xavista_UI_SOURCES}
  ${Xavista_MOC_SOURCES})
target_link_libraries(Xavista
  ${Xavista_LIBRARIES})

install(TARGETS Xavista
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH}
)

if(APPLE)
    set(PREPARE_BUNDLE ON)
    set(Xavista_EXECUTABLE "\${CMAKE_INSTALL_PREFIX}/Xavista.app")
    set(Xavista_QT_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}/Xavista.app/Contents/PlugIns/")
    install(CODE
      "FILE(
        INSTALL \"${Xavi_SOURCE_DIR}/Xavista/qt.conf\"
        DESTINATION \"\${CMAKE_INSTALL_PREFIX}/Xavista.app/Contents/Resources\"
       )"
    )
elseif(WIN32)
    set(PREPARE_BUNDLE ON)
    set(Xavista_EXECUTABLE "\${CMAKE_INSTALL_PREFIX}/Xavista.exe")
    set(Xavista_QT_PLUGIN_DIR "\${CMAKE_INSTALL_PREFIX}")
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
    install(CODE "FILE(INSTALL ${plugin} DESTINATION \"${Xavista_QT_PLUGIN_DIR}/${directory}\")")
    list(APPEND Xavista_PLUGINS "${Xavista_QT_PLUGIN_DIR}/${short}")
  endforeach()

  install(CODE "include(BundleUtilities)")
  install(CODE "fixup_bundle(\"${Xavista_EXECUTABLE}\" \"${Xavista_PLUGINS}\" \"${Library_BINARY_DIR}\")")
endif()
