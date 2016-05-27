# tests
include_directories(${Siliko_SOURCE_DIR})

add_executable(TestLexer TestLexer.c)
target_link_libraries(TestLexer Siliko)

add_executable(TestLinkage TestLinkage.c)
target_link_libraries(TestLinkage Siliko)
