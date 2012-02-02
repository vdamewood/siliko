# Xavista

set(QT_USE_QTMAIN TRUE)
include(${QT_USE_FILE})

set(Library_SOURCE_DIR ${Xavi_SOURCE_DIR}/Xavi)
set(Library_BINARY_DIR ${Xavi_BINARY_DIR}/Xavi)
set(Xavista_HEADERS ${Library_DIR}/Xavi.h)
set(Xavista_MOC_HEADERS XavistaWindow.h)
set(Xavista_SOURCES Xavista.cpp XavistaWindow.cpp)
set(Xavista_UIS XavistaWindow.ui)
set(Xavista_LIBRARIES Xavi ${QT_LIBRARIES})
set(Xavista_BUNDLES ${CMAKE_INSTALL_PREFIX}/Xavista.app)
set(Xavista_PLUGINS)
set(Xavista_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_LIBRARY_DIRS})

add_definitions(${QT_DEFINITIONS})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Xavi_SOURCE_DIR}/Xavi
  ${QT_INCLUDE_DIR})

qt4_wrap_ui(Xavista_UI_SOURCES ${Xavista_UIS})
qt4_wrap_cpp(Xavista_MOC_SOURCES ${Xavista_MOC_HEADERS})

add_executable(Xavista
  WIN32 MACOSX_BUNDLE
  ${Xavista_SOURCES}
  ${Xavista_UI_SOURCES}
  ${Xavista_MOC_SOURCES})
target_link_libraries(Xavista
  ${Xavista_LIBRARIES})

if(APPLE)
install(TARGETS Xavista
  BUNDLE DESTINATION . COMPONENT Runtime)
install(
  CODE
    "include(BundleUtilities)
    fixup_bundle(
      \"${Xavista_BUNDLES}\"
      \"${Xavista_PLUGINS}\"
      \"${Xavista_FRAMEWORK_LOCATIONS}\")"
  COMPONENT Runtime)
endif()
