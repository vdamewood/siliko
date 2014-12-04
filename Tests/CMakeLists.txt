# tests
set(LIBRARY_DIR ${Xavi_SOURCE_DIR}/Xavi)
include_directories(${Xavi_SOURCE_DIR})

add_executable(XaviTestFunctions
    XaviTestFunctions.cpp)
target_link_libraries(XaviTestFunctions Xavi ${MATH_LIB})

#add_executable(XaviTestHash
#    XaviTestHash.cpp)
#target_link_libraries(XaviTestHash Xavi)

add_executable(XaviTestLexer
    XaviTestLexer.cpp)
target_link_libraries(XaviTestLexer Xavi)

