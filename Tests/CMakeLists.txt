# tests
set(LIBRARY_DIR ${Xavi_SOURCE_DIR}/Xavi)
include_directories(${LIBRARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/../Xavi)

add_executable(XaviTestHash
    XaviTestHash.c)
target_link_libraries(XaviTestHash Xavi)

add_executable(XaviTestLinkage
    XaviTestLinkage.c)
target_link_libraries(XaviTestLinkage Xavi)

add_executable(XaviTestLexer
    XaviTestLexer.c)
target_link_libraries(XaviTestLexer Xavi)

add_executable(XaviTestFunctions
    XaviTestFunctions.c
    ${LIBRARY_DIR}/XaviFunctionCall.c)
target_link_libraries(XaviTestFunctions ${MATH_LIB})
