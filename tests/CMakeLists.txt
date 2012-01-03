INCLUDE_DIRECTORIES(${XVCALC_SOURCE_DIR}/libxvcalc)

add_executable(XvcTestLinkage
	XvcTestLinkage.c)
target_link_libraries(XvcTestLinkage xvcalc)

add_executable(XvcTestFunctions
	XvcTestFunctions.c
	${XVCALC_SOURCE_DIR}/libxvcalc/XvcFunctions.c)
