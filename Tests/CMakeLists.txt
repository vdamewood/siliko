# tests
include_directories(${Xavi_SOURCE_DIR})

add_executable(XaviTestFunctions
  XaviTestFunctions.cpp)
target_link_libraries(XaviTestFunctions Xavi++)

add_executable(XaviTestLexer
  XaviTestLexer.cpp)
target_link_libraries(XaviTestLexer Xavi++)
