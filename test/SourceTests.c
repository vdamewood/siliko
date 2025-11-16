#include <criterion/criterion.h>
#include <SilikoCore/DataSource.h>
#include <SilikoCore/StringSource.h>

#define SourceTest(NAME, STRING) \
Test(StringSourceTests, NAME) \
{ \
    char input[] = STRING; \
    SilikoDataSource *source = SilikoStringSourceNew(input); \
\
    for (int i = 0; i < sizeof input; i++) \
    { \
        cr_assert(SilikoDataSourceGet(source) == input[i]); \
        SilikoDataSourceAdvance(source); \
    } \
    cr_assert( \
        SilikoDataSourceGet(source) == '\0' \
        && input[sizeof input - 1] == '\0' \
    ); \
    SilikoDataSourceDelete(source); \
}

SourceTest(EmptryString, "")
SourceTest(RandomString, "nk4jsormahlr0493+%@^%!#@@#$@##$^")
SourceTest(PlausibleExpression, "2 + 3 * 4.5^abs(3d6) - 5 / 1")
