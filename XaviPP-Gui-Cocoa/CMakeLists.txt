set(EXECUTABLE XaviPP-Gui-Cocoa)

string(CONCAT CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -fobjc-arc")

set(HEADERS Delegate.hh)
set(SOURCES Delegate.mm XaviPP-Gui-Cocoa.m)
set(XIB MainMenu)

set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.XaviPP-Gui-Cocoa")
set(MACOSX_BUNDLE_BUNDLE_NAME "XaviPP")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${Xavi_VERSION_STRING})
set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0)
set(MACOSX_BUNDLE_COPYRIGHT ${Xavi_APP_COPYRIGHT_STATEMENT})
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")

add_executable("${EXECUTABLE}"
    MACOSX_BUNDLE
    ${HEADERS}
    ${SOURCES}
)

set_target_properties("${EXECUTABLE}" PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
        "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)

target_link_libraries("${EXECUTABLE}" XaviPP "-framework Cocoa")

add_custom_command(
	TARGET "${EXECUTABLE}" PRE_LINK
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.app/Contents/Resources)

add_custom_command(
	TARGET "${EXECUTABLE}" POST_BUILD
	COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
		--compile
		${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.app/Contents/Resources/${XIB}.nib
		${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib
        COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib")

install(TARGETS "${EXECUTABLE}"
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH}
)

install(CODE "include(BundleUtilities)")
install(CODE "fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE}.app\" \"\" \"${XaviPP_BINARY_DIR}\")")
