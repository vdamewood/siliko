#cmake_minimum_required(VERSION 3.0)
#project(Xavi_Gui_Cocoa)

set(Xavi_Gui_Cocoa_HEADERS
	Delegate.h)

set(Xavi_Gui_Cocoa_SOURCES
	Delegate.m
	Xavi-Gui-Cocoa.m)

set(Xavi_Gui_Cocoa_XIB
	MainMenu)

string(CONCAT CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " -fobjc-arc")

set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.Xavi-Gui-Cocoa")
set(MACOSX_BUNDLE_BUNDLE_NAME "Xavi")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${XAVI_VERSION_MAJOR}.${XAVI_VERSION_MINOR}.${XAVI_VERSION_PATCH}")
set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0)
set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2012, 2014, 2015 Vincent Damewood. Licensed under the terms of the GNU General Public License, version 3 or later. See the file License.txt for more information.")
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${Xavi_Gui_Cocoa_XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")

add_executable(
	Xavi-Gui-Cocoa MACOSX_BUNDLE
	${Xavi_Gui_Cocoa_SOURCES}
	${Xavi_Gui_Cocoa_HEADERS}
)

set_target_properties(Xavi-Gui-Cocoa Xavi PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)

target_link_libraries(Xavi-Gui-Cocoa Xavi "-framework Cocoa")

add_custom_command(
	TARGET Xavi-Gui-Cocoa PRE_LINK
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Xavi-Gui-Cocoa.app/Contents/Resources)

add_custom_command(
	TARGET Xavi-Gui-Cocoa POST_BUILD
	COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
		--compile
		${CMAKE_CURRENT_BINARY_DIR}/Xavi-Gui-Cocoa.app/Contents/Resources/${Xavi_Gui_Cocoa_XIB}.nib
		${CMAKE_CURRENT_SOURCE_DIR}/${Xavi_Gui_Cocoa_XIB}.xib
        COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${Xavi_Gui_Cocoa_XIB}.xib")

install(TARGETS Xavi-Gui-Cocoa
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH}
)
