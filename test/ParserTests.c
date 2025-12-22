#include <criterion/criterion.h>
#include <SilikoCore/InfixParser.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/SyntaxTree.h>

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
	SilikoDataSource * src = SilikoStringSourceNew(Input); \
	cr_assert(src != NULL); \
	SilikoFunctionCaller *caller = SilikoFunctionCallerNew(); \
	cr_assert(caller != NULL); \
	SilikoFunctionCallerInstallOperators(caller); \
	SilikoFunctionCallerInstallFunctions(caller); \
\
	SilikoSyntaxTreeNode *tree = SilikoParseInfix(src); \
    SilikoValue *target = SilikoValueNew(TARGET); \
	SilikoValue *result = SilikoSyntaxTreeEvaluate(tree, caller); \
    cr_assert(SilikoValueGetStatus(target) \
        == SilikoValueGetStatus(result), \
        "Value status shoule Be: %d Is: %d: %d", \
            SilikoValueGetStatus(target), \
            SilikoValueGetStatus(result), \
            SilikoValueToError(result)); \
    switch(SilikoValueGetStatus(result)) \
    { \
    case SilikoValueInteger: \
        cr_assert(SilikoValueToInteger(target) == SilikoValueToInteger(result), \
            "Integer mismatch: should be: %lld is: %lld", \
            SilikoValueToInteger(target), \
            SilikoValueToInteger(result)); \
        break; \
    case SilikoValueReal: \
        cr_assert(SilikoValueToReal(target) == SilikoValueToReal(result), \
            "Real mismatch: should be: %f is: %f", \
            SilikoValueToReal(target), \
            SilikoValueToReal(result)); \
        break; \
    default: \
        break; \
    } \
	SilikoSyntaxTreeDelete(tree); \
}

ParserTest(Test0000_AnInteger, "42", 42)
ParserTest(Test0001_AFloat, "42.125", 42.125)
ParserTest(Test0002_JustAFunction, "abs(3)", 3)
ParserTest(Test0003_TwoPlusThreeIsfive, "2 + 3", 5)
ParserTest(Test0004_TwoTimesThreeIsSix, "2 * 3", 6)
ParserTest(Test0005_TwoPlusThreeIsfiveWithFloats, "2.0 + 3.0", 5.0)
ParserTest(Test0006_FunctionAndParens, "sin(0.0) + 4", 4.0)
ParserTest(Test0007_LotsOfTimes, "multiply(2, 3, 5)", 30)
