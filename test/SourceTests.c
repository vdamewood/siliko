#include <criterion/criterion.h>
#include <SilikoCore/Input.h>
#include <SilikoCore/StringInput.h>

#define SourceTest(NAME, STRING) \
Test(StringSourceTests, NAME) \
{ \
    char input[] = STRING; \
    SilikoInput *source = SilikoStringInputNew(input); \
\
    for (int i = 0; i < sizeof input; i++) \
    { \
        cr_assert(SilikoInputGetCharacter(source) == input[i]); \
        SilikoInputAdvance(source); \
    } \
    cr_assert( \
        SilikoInputGetCharacter(source) == '\0' \
        && input[sizeof input - 1] == '\0' \
    ); \
    SilikoInputDelete(source); \
}

SourceTest(EmptryString, "")
SourceTest(RandomString, "nk4jsormahlr0493+%@^%!#@@#$@##$^")
SourceTest(PlausibleExpression, "2 + 3 * 4.5^abs(3d6) - 5 / 1")
