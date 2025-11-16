project(siliko-test
    VERSION   0.0.0
    LANGUAGES C
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Criterion REQUIRED criterion IMPORTED_TARGET)

add_executable(SilikoTester)
target_sources(SilikoTester
PRIVATE
    FunctionMacros.h
    SourceTests.c
    Value.c
    Caller.c
    Functions.c
    FunctionsTrig.c
    FunctionsArc.c
    FunctionsHyper.c
    FunctionsOps.c
    Node.c
    LexerTests.c
    ParserTests.c
)
target_link_libraries(SilikoTester
PRIVATE
    Siliko::SilikoCore
    PkgConfig::Criterion
)
add_test(NAME Tests
    COMMAND SilikoTester --verbose --color
)
