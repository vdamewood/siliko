# Xavista

set(Library_SOURCE_DIR ${Xavi_SOURCE_DIR}/Xavi)
set(Library_BINARY_DIR ${Xavi_BINARY_DIR}/Xavi)
set(Xavista_HEADERS ${Library_DIR}/Xavi.h)
set(Xavista_MOC_HEADERS XavistaWindow.h)
set(Xavista_SOURCES Xavista.cpp XavistaWindow.cpp)
set(Xavista_UIS XavistaWindow.ui)
set(Xavista_LIBRARIES Xavi Qt5::Widgets)
set(Xavista_PLUGINS)
if (MACOSX)
set(Xavista_BUNDLES "\${CMAKE_INSTALL_PREFIX}/Xavista.app")
set(Xavista_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_LIBRARY_DIR})
elseif (WIN32)
set(Xavista_BUNDLES "\${CMAKE_INSTALL_PREFIX}/Xavista.exe")
set(Xavista_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_BINARY_DIR})
else()
set(Xavista_BUNDLES "")
endif()

add_definitions(${QT_DEFINITIONS})

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

if(MACOSX OR WIN32)
  install(
    CODE
      "include(BundleUtilities)
	  message(\"Bundle: ${Xavista_BUNDLES}\")
      fixup_bundle(
        \"${Xavista_BUNDLES}\"
        \"${Xavista_PLUGINS}\"
        \"${Xavista_FRAMEWORK_LOCATIONS}\")"
    )
endif()
