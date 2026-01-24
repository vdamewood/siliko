#include <criterion/criterion.h>
#include <SilikoCore/ParseInfix.h>
#include <SilikoCore/Input.h>
#include <SilikoCore/Node.h>
#include <SilikoCore/Evaluate.h>
#include <SilikoCore/Operation.h>
#include <SilikoCore/Math.h>

#include "Macros.h"

#define InitValue(VALUE) _Generic((VALUE), \
    signed char:            SilikoValueInitInteger, \
    short int:              SilikoValueInitInteger, \
    int:                    SilikoValueInitInteger, \
    long int:               SilikoValueInitInteger, \
    long long int:          SilikoValueInitInteger, \
    float:                  SilikoValueInitFloat,   \
    double:                 SilikoValueInitFloat,   \
    default:                SilikoValueInitInteger  \
)(VALUE)

#define ParserTest(NAME, INPUT, TARGET) \
Test(ParserTests, NAME) \
{ \
	const char Input[] = INPUT; \
	SilikoInput * src = SilikoStringInputCreate(Input); \
	cr_assert(src != NULL); \
	SilikoEngine *caller = SilikoEngineCreate(); \
	cr_assert(caller != NULL); \
	SilikoInstallOperations(caller); \
	SilikoInstallMathFunctions(caller); \
\
	SilikoNode *tree = SilikoParseInfix(src); \
    SilikoValue *target = SilikoValueCreate(TARGET); \
	SilikoValue *result = SilikoEvaluate(caller, tree); \
    cr_assert(SilikoValueGetStatus(target) \
        == SilikoValueGetStatus(result), \
        "Value status shoule Be: %d Is: %d: %d", \
            SilikoValueGetStatus(target), \
            SilikoValueGetStatus(result), \
            SilikoValueGetError(result)); \
    switch(SilikoValueGetStatus(result)) \
    { \
    case SilikoValueInteger: \
        cr_assert(SilikoValueGetInteger(target) == SilikoValueGetInteger(result), \
            "Integer mismatch: should be: %lld is: %lld", \
            SilikoValueGetInteger(target), \
            SilikoValueGetInteger(result)); \
        break; \
    case SilikoValueReal: \
        cr_assert(SilikoValueGetReal(target) == SilikoValueGetReal(result), \
            "Real mismatch: should be: %f is: %f", \
            SilikoValueGetReal(target), \
            SilikoValueGetReal(result)); \
        break; \
    default: \
        break; \
    } \
	SilikoNodeDestroy(tree); \
}

ParserTest(Test0000_AnInteger, "42", 42)
ParserTest(Test0001_AFloat, "42.125", 42.125)
ParserTest(Test0002_JustAFunction, "abs(3)", 3)
ParserTest(Test0003_TwoPlusThreeIsfive, "2 + 3", 5)
ParserTest(Test0004_TwoTimesThreeIsSix, "2 * 3", 6)
ParserTest(Test0005_TwoPlusThreeIsfiveWithFloats, "2.0 + 3.0", 5.0)
ParserTest(Test0006_FunctionAndParens, "sin(0.0) + 4", 4.0)
ParserTest(Test0007_LotsOfTimes, "multiply(2, 3, 5)", 30)
