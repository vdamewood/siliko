# XvcGui

set(QT_USE_QTMAIN TRUE)
include(${QT_USE_FILE})

set(Library_SOURCE_DIR ${XVCalc_SOURCE_DIR}/libxvcalc)
set(Library_BINARY_DIR ${XVCalc_BINARY_DIR}/libxvcalc)
set(XvcGui_HEADERS ${Library_DIR}/XVCalc.h)
set(XvcGui_MOC_HEADERS XvcWindow.h)
set(XvcGui_SOURCES XvcGui.cpp XvcWindow.cpp)
set(XvcGui_UIS XvcWindow.ui)
set(XvcGui_LIBRARIES XVCalc ${QT_LIBRARIES})
set(XvcGui_BUNDLES ${CMAKE_INSTALL_PREFIX}/XvcGui.app)
set(XvcGui_PLUGINS)
set(XvcGui_FRAMEWORK_LOCATIONS ${Library_BINARY_DIR} ${QT_LIBRARY_DIRS})

add_definitions(${QT_DEFINITIONS})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XVCalc_SOURCE_DIR}/libxvcalc
  ${QT_INCLUDE_DIR})

qt4_wrap_ui(XvcGui_UI_SOURCES ${XvcGui_UIS})
qt4_wrap_cpp(XvcGui_MOC_SOURCES ${XvcGui_MOC_HEADERS})

add_executable(XvcGui
  WIN32 MACOSX_BUNDLE
  ${XvcGui_SOURCES}
  ${XvcGui_UI_SOURCES}
  ${XvcGui_MOC_SOURCES})
target_link_libraries(XvcGui
  ${XvcGui_LIBRARIES})

if(APPLE)
install(TARGETS XvcGui
  BUNDLE DESTINATION . COMPONENT Runtime)
install(
  CODE
    "include(BundleUtilities)
    fixup_bundle(
      \"${XvcGui_BUNDLES}\"
      \"${XvcGui_PLUGINS}\"
      \"${XvcGui_FRAMEWORK_LOCATIONS}\")"
  COMPONENT Runtime)
endif()
