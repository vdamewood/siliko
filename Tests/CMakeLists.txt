# tests
include_directories(${Xavi_SOURCE_DIR})

add_executable(TestXaviLexer
	TestXaviLexer.c)
target_link_libraries(TestXaviLexer Xavi)

add_executable(TestXaviLinkage
	TestXaviLinkage.c)
target_link_libraries(TestXaviLinkage Xavi)
