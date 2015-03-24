# tests
include_directories(${Xavi_SOURCE_DIR})

add_executable(TestXaviLexer
	TestXaviLexer.c)
target_link_libraries(TestXaviLexer Xavi)

add_executable(TestXaviLinkage
	TestXaviLinkage.c)
target_link_libraries(TestXaviLinkage Xavi)

add_executable(TestXaviPPFunctions
	TestXaviPPFunctions.cpp)
target_link_libraries(TestXaviPPFunctions XaviPP)

add_executable(TestXaviPPLexer
	TestXaviPPLexer.cpp)
target_link_libraries(TestXaviPPLexer XaviPP)
