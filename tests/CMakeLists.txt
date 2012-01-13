INCLUDE_DIRECTORIES(${XVCalc_SOURCE_DIR}/libxvcalc)

add_executable(XvcTestLinkage XvcTestLinkage.c)
target_link_libraries(XvcTestLinkage xvcalc)

add_executable(XvcTestFunctions
	XvcTestFunctions.c ${XVCalc_SOURCE_DIR}/libxvcalc/XvcFunctionCall.c)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(XvcTestFunctions ${MATH_LIB})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
