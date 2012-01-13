#FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE (${QT_USE_FILE})

INCLUDE_DIRECTORIES (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	${XVCalc_SOURCE_DIR}/libxvcalc .)
SET (XvcGui_HDRS ${XVCalc_SOURCE_DIR}/libxvcalc/XVCalc.h)
SET (XvcGui_SRCS XvcGui.cpp XvcWindow.cpp)
SET (XvcGui_UIS  XvcWindow.ui)
SET (XvcGui_MOCS XvcWindow.h)

QT4_WRAP_UI(UIS ${XvcGui_UIS})
QT4_WRAP_CPP(MOCS ${XvcGui_MOCS})

add_executable ( XvcGui ${XvcGui_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( XvcGui xvcalc ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )
install (TARGETS XvcGui DESTINATION bin)
