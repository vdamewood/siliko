# XvcGui
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XVCalc_SOURCE_DIR}/libxvcalc
  ${QT_INCLUDE_DIR})

set(XvcGui_HEADERS ${XVCalc_SOURCE_DIR}/libxvcalc/XVCalc.h)
set(XvcGui_MOC_HEADERS XvcWindow.h)
set(XvcGui_SOURCES XvcGui.cpp XvcWindow.cpp)
set(XvcGui_UIS XvcWindow.ui)

qt4_wrap_ui(XvcGui_UI_SOURCES ${XvcGui_UIS})
qt4_wrap_cpp(XvcGui_MOC_SOURCES ${XvcGui_MOC_HEADERS})

set(QT_USE_QTMAIN TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(XvcGui
  WIN32 MACOSX_BUNDLE
  ${XvcGui_SOURCES}
  ${XvcGui_UI_SOURCES}
  ${XvcGui_MOC_SOURCES})
target_link_libraries(XvcGui
  XVCalc
  ${QT_LIBRARIES})
