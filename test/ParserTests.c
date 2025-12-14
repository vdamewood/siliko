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
        == SilikoValueGetStatus(result)); \
    switch(SilikoValueGetStatus(result)) \
    { \
    case SilikoValueInteger: \
        cr_assert(SilikoValueToInteger(target) == SilikoValueToInteger(result)); \
        break; \
    case SilikoValueReal: \
        cr_assert(SilikoValueToReal(target) == SilikoValueToReal(result)); \
        break; \
    default: \
        break; \
    } \
	SilikoSyntaxTreeDelete(tree); \
}

ParserTest(TwoPlusThreeIsfive, "2 + 3", 5)
ParserTest(TwoPlusThreeIsfiveWithFloats, "2.0 + 3.0", 5.0)
ParserTest(FunctionAndParens, "sin(0.0) + 4", 4.0)
ParserTest(LostOfTimes, "multiply(2, 3, 5)", 30)
