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

set(EXENAME XaviPP-Gui-Cocoa)
set(HDRS Delegate.hh)
set(XIB MainMenu)
set(SRCS Delegate.mm XaviPP-Gui-Cocoa.m "${XIB}.nib")
set(LIBS ${COCOA} ${CXXLIB})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.${EXENAME}")
set(MACOSX_BUNDLE_BUNDLE_NAME "Xavi")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${Xavi_VERSION_STRING})
set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0)
set(MACOSX_BUNDLE_COPYRIGHT ${Xavi_APP_COPYRIGHT_STATEMENT})
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")

add_custom_command(OUTPUT "${XIB}.nib"
	COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
		--compile
		${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}.app/Contents/Resources/${XIB}.nib
		${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib
        COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib")

add_executable(${EXENAME} MACOSX_BUNDLE ${HDRS} ${SRCS})
target_link_libraries(${EXENAME} ${LIBS})
set_target_properties(${EXENAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
install(TARGETS "${EXENAME}"
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})

install(CODE "include(BundleUtilities)")
install(CODE "fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${EXENAME}.app\" \"\" \"${CXXLIB_BINARY_DIR}\")")
