#include <criterion/criterion.h>
#include <SilikoCore/FunctionCaller.h>

#define SUITE FunctionOperatorTests
#define INSTALL_FUNCTION SilikoFunctionCallerInstallOperators
#include "FunctionMacros.h"

TestFunctionForEquality(TwoPlusThreeIsFive,
    "add", 5,
    SILIKO_VALUE(2),
    SILIKO_VALUE(3),
)

TestFunctionForEquality(TwoPlusThreeIsFiveWithFloats,
    "add", 5.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOne,
    "add", -1,
    SILIKO_VALUE(2),
    SILIKO_VALUE(-3),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOneWithFloats,
    "add", -1.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(-3.0),
)

TestFunctionForEquality(OneEighthPlusHalfIsFiveEighths,
    "add", 0.625,
    SILIKO_VALUE(0.125),
    SILIKO_VALUE(0.5),
)

TestFunctionForEquality(FiveIntegerCanBeAdded,
    "add", 28,
    SILIKO_VALUE(2),
    SILIKO_VALUE(3),
    SILIKO_VALUE(5),
    SILIKO_VALUE(7),
    SILIKO_VALUE(11),
)

TestFunctionForEquality(FiveFloatsCanBeAdded,
    "add", 28.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0),
    SILIKO_VALUE(5.0),
    SILIKO_VALUE(7.0),
    SILIKO_VALUE(11.0),
)

TestFunctionForEquality(AddingMixedTypesGivesFloats,
    "add", 10.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0),
    SILIKO_VALUE(5.0),
)

TestFunctionForEquality(ThreeMinusTwoIsOne,
    "subtract", 1,
    SILIKO_VALUE(3),
    SILIKO_VALUE(2),
)

TestFunctionForEquality(ThreeMinusTwoIsOneWithFloats,
    "subtract", 1.0,
    SILIKO_VALUE(3.0),
    SILIKO_VALUE(2.0),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOne,
    "subtract", -1,
    SILIKO_VALUE(2),
    SILIKO_VALUE(3),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOneWithFloats,
    "subtract", -1.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0),
)

TestFunctionForEquality(SubtractingMixedNumbersGivesFloat,
    "subtract", 19.5,
    SILIKO_VALUE(23),
    SILIKO_VALUE(3.5),
)

TestFunctionForEquality(SubtractingMultipleIntegersDoesntCrash,
    "subtract", 4,
    SILIKO_VALUE(100),
    SILIKO_VALUE(50),
    SILIKO_VALUE(25),
    SILIKO_VALUE(12),
    SILIKO_VALUE(6),
    SILIKO_VALUE(3),
)

TestFunctionForEquality(SubtractingMultipleFloatsDoesntCrash,
    "subtract", 3.125,
    SILIKO_VALUE(100.0),
    SILIKO_VALUE(50.0),
    SILIKO_VALUE(25.0),
    SILIKO_VALUE(12.5),
    SILIKO_VALUE(6.25),
    SILIKO_VALUE(3.125),
)

TestFunctionForEquality(TwoTimesThreeIsSix,
    "multiply", 6,
    SILIKO_VALUE(2),
    SILIKO_VALUE(3)
)

TestFunctionForEquality(TwoTimesThreeIsSixWithFloats,
    "multiply", 6.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTen,
    "multiply", -10,
    SILIKO_VALUE(2),
    SILIKO_VALUE(-5)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTenWithFloats,
    "multiply", -10.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(-5.0),
)

TestFunctionForEquality(TwoWrongsMakeARight,
    "multiply", 70,
    SILIKO_VALUE(-7),
    SILIKO_VALUE(-10),
)

TestFunctionForEquality(TwoWrongsMakeARightWithFloats,
    "multiply", 70.0,
    SILIKO_VALUE(-7.0),
    SILIKO_VALUE(-10.0),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrash,
    "multiply", 60,
    SILIKO_VALUE(2),
    SILIKO_VALUE(3),
    SILIKO_VALUE(5),
    SILIKO_VALUE(2),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrashWithFloats,
    "multiply", 60.0,
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(3.0),
    SILIKO_VALUE(5.0),
    SILIKO_VALUE(2.0),
)

TestFunctionForEquality(MultiplyingIntegerAndFloatIsFloat,
    "multiply", 30.0,
    SILIKO_VALUE(2),
    SILIKO_VALUE(15.0),
)

TestFunctionForEquality(TenDividedByTwoIsFive,
    "divide", 5,
    SILIKO_VALUE(10),
    SILIKO_VALUE(2)
)

TestFunctionForEquality(TenDividedByTwoIsFiveWithFloats,
    "divide", 5.0,
    SILIKO_VALUE(10.0),
    SILIKO_VALUE(2.0)
)

TestFunctionForEquality(MultipleDividesDontCrash,
    "divide", 1,
    SILIKO_VALUE(100),
    SILIKO_VALUE(25),
    SILIKO_VALUE(2),
    SILIKO_VALUE(2)
)

TestFunctionForEquality(MultipleDividesDontCrashWithFloats,
    "divide", 1.0,
    SILIKO_VALUE(100.0),
    SILIKO_VALUE(25.0),
    SILIKO_VALUE(2.0),
    SILIKO_VALUE(2.0)
)

TestFunctionForEquality(IntegersWillDivideToFloat,
    "divide", 2.5,
    SILIKO_VALUE(10),
    SILIKO_VALUE(4)
)

TestFunctionForEquality(DividingIntByFloatWorks,
    "divide", 5.0,
    SILIKO_VALUE(10),
    SILIKO_VALUE(2.0)
)

TestFunctionForEquality(TwoToTheTwoIsFour,
    "power", 4.0,
    SILIKO_VALUE(2),
    SILIKO_VALUE(2)
)

TestFunctionForEquality(OneHundredToTheHalfIsTen,
    "power", 10.0,
    SILIKO_VALUE(100.0),
    SILIKO_VALUE(0.5)
)
