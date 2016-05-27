# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

set(EXENAME Siliko-Gui)
set(HDRS Delegate.h)
set(XIB MainMenu)
set(SRCS Delegate.m Main.m "${XIB}.nib")
set(LIBS ${COCOA} ${CLIB})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-arc")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.${EXENAME}")
set(MACOSX_BUNDLE_BUNDLE_NAME "Siliko")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${Siliko_VERSION_STRING})
set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0)
set(MACOSX_BUNDLE_COPYRIGHT ${Siliko_APP_COPYRIGHT_STATEMENT})
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")

add_custom_command(OUTPUT "${XIB}.nib"
	COMMAND ${IBTOOL} --compile ${XIB}.nib ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib
        COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib")
set_source_files_properties("${XIB}.nib" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(${EXENAME} MACOSX_BUNDLE ${HDRS} ${SRCS})
target_link_libraries(${EXENAME} ${LIBS})
set_target_properties(${EXENAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
install(TARGETS "${EXENAME}"
  RUNTIME DESTINATION ${RUNTIME_INSTALL_PATH}
  BUNDLE  DESTINATION ${BUNDLE_INSTALL_PATH})

install(CODE "include(BundleUtilities)")
install(CODE "fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${EXENAME}.app\" \"\" \"${CLIB_BINARY_DIR}\")")
