# tests
include_directories(${XVCalc_SOURCE_DIR}/libxvcalc)

add_executable(XvcTestLinkage
    XvcTestLinkage.c)
target_link_libraries(XvcTestLinkage XVCalc)

add_executable(XvcTestFunctions
    XvcTestFunctions.c
    ${XVCalc_SOURCE_DIR}/libxvcalc/XvcFunctionCall.c)
target_link_libraries(XvcTestFunctions ${MATH_LIB})
