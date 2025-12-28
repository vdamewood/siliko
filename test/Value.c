#include <criterion/criterion.h>
#include <SilikoCore/Value.h>

#include "Macros.h"

#define VALUE_TEST(TEST_NAME, VALUE) \
Test(ValueTests, TEST_NAME) \
{ \
    SilikoValue *test_value = SilikoValueNew(VALUE); \
    cr_assert(test_value); \
    cr_assert(SilikoValueGetStatus(test_value) == _Generic((VALUE), \
        int:            SilikoValueInteger, \
        long long int:  SilikoValueInteger, \
        double:         SilikoValueReal \
    )); \
    cr_assert(_Generic((VALUE), \
        int:            SilikoValueGetInteger, \
        long long int:  SilikoValueGetInteger, \
        double:         SilikoValueGetReal \
    )(test_value) == VALUE); \
    SilikoValueDelete(test_value); \
}

// Errors require a separate macro because `enum`s
// look like `int`s to the preprocessor.
#define VALUE_ERROR_TEST(TEST_NAME, VALUE) \
Test(ValueTests, TEST_NAME) \
{ \
    SilikoValue *test_value = SilikoValueNewFromError(VALUE); \
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueError); \
    cr_assert(SilikoValueGetError(test_value) == VALUE); \
    SilikoValueDelete(test_value); \
}

VALUE_TEST(NewIntZero, 0LL)
VALUE_TEST(NewIntFortyTwo, 42LL)
VALUE_TEST(NewIntNegativeOne, -1LL)
VALUE_TEST(NewRealZero, 0.0)
VALUE_TEST(NewRealOneeighth, 0.125)
VALUE_TEST(NewRealNegativeZero, -0.0)
VALUE_TEST(NewRealFortyTwo, 42.0)
VALUE_TEST(NewRealNegativeOne, -1.0)

VALUE_ERROR_TEST(NewSyntaxError, SilikoErrorSyntax)
VALUE_ERROR_TEST(NewFunctionNameError, SilikoErrorFunctionName)
VALUE_ERROR_TEST(NewFunctionArgumentsError, SilikoErrorFunctionArguments)
VALUE_ERROR_TEST(NewDomainError, SilikoErrorDomain)
VALUE_ERROR_TEST(NewRangeError, SilikoErrorRange)
