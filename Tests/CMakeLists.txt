# tests
set(LIBRARY_DIR ${Xavi_SOURCE_DIR}/Xavi)
include_directories(${LIBRARY_DIR})

add_executable(XaviTestLinkage
    XaviTestLinkage.c)
target_link_libraries(XaviTestLinkage Xavi)

add_executable(XaviTestFunctions
    XaviTestFunctions.c
    ${LIBRARY_DIR}/XaviFunctionCall.c)
target_link_libraries(XaviTestFunctions ${MATH_LIB})
