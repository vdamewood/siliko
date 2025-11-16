#include <criterion/criterion.h>
#include <SilikoCore/Value.h>

Test(ValueTests, NewIntZero) {
    struct SilikoValue Value = SILIKO_VALUE(0LL);
    cr_assert(Value.Status == SILIKO_VAL_INTEGER);
    cr_assert(Value.Integer == 0LL);
}

Test(ValueTests, NewIntFortyTwo) {
    struct SilikoValue Value = SILIKO_VALUE(42LL);
    cr_assert(Value.Status == SILIKO_VAL_INTEGER);
    cr_assert(Value.Integer == 42LL);
}

Test(ValueTests, NewIntNegativeOne) {
    struct SilikoValue Value = SILIKO_VALUE(-1LL);
    cr_assert(Value.Status == SILIKO_VAL_INTEGER);
    cr_assert(Value.Integer == -1LL);
}

Test(ValueTests, NewFloatZero) {
    struct SilikoValue Value = SILIKO_VALUE(0.0);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Integer == 0.0);
}

Test(ValueTests, NewFloatNegativeZero) {
    struct SilikoValue Value = SILIKO_VALUE(-0.0);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Float == -0.0);
}

Test(ValueTests, NewFloatOneEighth) {
    struct SilikoValue Value = SILIKO_VALUE(0.125);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Float == 0.125);
}

Test(ValueTests, NewFloatFortyTwo) {
    struct SilikoValue Value = SILIKO_VALUE(42.0);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Float == 42.0);
}

Test(ValueTests, NewFloatNegativeOne) {
    struct SilikoValue Value = SILIKO_VALUE(-1.0);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Float == -1.0);
}

Test(ValueTests, NewErrorMemory)
{
    struct SilikoValue Value = {SILIKO_VAL_MEMORY_ERR};
	cr_assert(Value.Status == SILIKO_VAL_MEMORY_ERR, "should: %d; is: %d; int: %lld", SILIKO_VAL_MEMORY_ERR, Value.Status, Value.Integer);
}

Test(ValueTests, NewErrorSyntax)
{
    struct SilikoValue Value = {SILIKO_VAL_SYNTAX_ERR};
	cr_assert(Value.Status == SILIKO_VAL_SYNTAX_ERR);
}

Test(ValueTests, NewErrorFunctionLookup)
{
    struct SilikoValue Value = {SILIKO_VAL_BAD_FUNCTION};
	cr_assert(Value.Status == SILIKO_VAL_BAD_FUNCTION);
}

Test(ValueTests, NewErrorFunctionArguments)
{
    struct SilikoValue Value = {SILIKO_VAL_BAD_ARGUMENTS};
	cr_assert(Value.Status == SILIKO_VAL_BAD_ARGUMENTS);
}

Test(ValueTests, NewErrorFunctionDomain)
{
    struct SilikoValue Value = {SILIKO_VAL_DOMAIN_ERR};
	cr_assert(Value.Status == SILIKO_VAL_DOMAIN_ERR);
}

Test(ValueTests, NewErrorFunctionRange)
{
    struct SilikoValue Value = {SILIKO_VAL_RANGE_ERR};
	cr_assert(Value.Status == SILIKO_VAL_RANGE_ERR);
}
