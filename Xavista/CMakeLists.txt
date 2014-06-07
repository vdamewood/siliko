# Xavista

set(Library_SOURCE_DIR  ${Xavi_SOURCE_DIR}/Xavi)
set(Library_BINARY_DIR  ${Xavi_BINARY_DIR}/Xavi)
set(Xavista_HEADERS     ${Library_SOURCE_DIR}/Xavi.h)
set(Xavista_MOC_HEADERS XavistaWindow.h)
set(Xavista_SOURCES     Xavista.cpp XavistaWindow.cpp)
set(Xavista_UIS         XavistaWindow.ui)
set(Xavista_LIBRARIES   Xavi Qt5::Widgets)
set(Xavista_PLUGINS)

if(APPLE)
	set(Xavista_BUNDLES             "\${CMAKE_INSTALL_PREFIX}/Xavista.app")
	set(Xavista_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_LIBRARY_DIR})
elseif(WIN32)
	set(Xavista_BUNDLES             "\${CMAKE_INSTALL_PREFIX}/Xavista.exe")
	set(Xavista_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_BINARY_DIR})
else()
	set(Xavista_BUNDLES "")
endif()

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Xavi_SOURCE_DIR}/Xavi
  ${QT_INCLUDE_DIR})

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

# This is actually only for APPLE right now
if(APPLE OR WIN32)
  foreach(plugin_target ${Qt5Gui_PLUGINS})
    get_target_property(plugin ${plugin_target} LOCATION)
    string(FIND "${plugin}" "plugins/" idx REVERSE)
    math(EXPR idx "${idx} + 8")
    string(SUBSTRING "${plugin}" "${idx}" -1 short)
    string(FIND ${short} "/" idxs REVERSE)
    string(SUBSTRING ${short} 0 ${idxs} directory)
    install(CODE "FILE(INSTALL ${plugin} DESTINATION \"${Xavista_BUNDLES}/Contents/PlugIns/${directory}\")")
    list(APPEND Xavista_PLUGINS "${Xavista_BUNDLES}/Contents/PlugIns/${short}")
  endforeach()

  install(CODE "FILE(INSTALL \"${Xavi_SOURCE_DIR}/Xavista/qt.conf\" DESTINATION \"${Xavista_BUNDLES}/Contents/Resources\")")
  install(CODE "include(BundleUtilities)")
  install(CODE "fixup_bundle(\"${Xavista_BUNDLES}\" \"${Xavista_PLUGINS}\" \"${Xavista_FRAMEWORK_LOCATIONS}\")")
endif()
