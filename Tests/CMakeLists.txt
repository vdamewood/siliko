# tests
include_directories(${Xavi_SOURCE_DIR})

add_executable(TestXaviLexer
	TestXaviLexer.c)
target_link_libraries(TestXaviLexer Xavi)

add_executable(TestXavi++Functions
	TestXavi++Functions.cpp)
target_link_libraries(TestXavi++Functions Xavi++)

add_executable(TestXavi++Lexer
	TestXavi++Lexer.cpp)
target_link_libraries(TestXavi++Lexer Xavi++)
