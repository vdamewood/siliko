#include <criterion/criterion.h>
#include <SilikoCore/InfixParser.h>
#include <SilikoCore/StringSource.h>
#include <SilikoCore/SyntaxTree.h>

#define InitValue(VALUE) _Generic((VALUE), \
    signed char:            SilikoValueInitInteger, \
    short int:              SilikoValueInitInteger, \
    int:                    SilikoValueInitInteger, \
    long int:               SilikoValueInitInteger, \
    long long int:          SilikoValueInitInteger, \
    float:                  SilikoValueInitFloat,   \
    double:                 SilikoValueInitFloat,   \
    enum SilikoValueStatus: SilikoValueInitStatus,  \
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
    struct SilikoValue target = SILIKO_VALUE(TARGET); \
	struct SilikoValue result = SilikoSyntaxTreeEvaluate(tree, caller); \
    cr_assert(target.Status == result.Status); \
    switch(result.Status) \
    { \
    case SILIKO_VAL_INTEGER: \
        cr_assert(target.Integer == result.Integer); \
        break; \
    case SILIKO_VAL_FLOAT: \
        cr_assert(target.Float == result.Float); \
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
