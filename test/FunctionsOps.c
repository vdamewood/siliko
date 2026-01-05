#include <criterion/criterion.h>
#include <SilikoCore/Engine.h>

#define SUITE FunctionOperatorTests
#define INSTALL_FUNCTION SilikoInstallOperators

#include "Macros.h"
#include "FunctionMacros.h"

TestFunctionForEquality(TwoPlusThreeIsFive,
    "add", 5,
    SilikoValueNew(2),
    SilikoValueNew(3),
)

TestFunctionForEquality(TwoPlusThreeIsFiveWithFloats,
    "add", 5.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOne,
    "add", -1,
    SilikoValueNew(2),
    SilikoValueNew(-3),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOneWithFloats,
    "add", -1.0,
    SilikoValueNew(2.0),
    SilikoValueNew(-3.0),
)

TestFunctionForEquality(OneEighthPlusHalfIsFiveEighths,
    "add", 0.625,
    SilikoValueNew(0.125),
    SilikoValueNew(0.5),
)

TestFunctionForEquality(FiveIntegerCanBeAdded,
    "add", 28,
    SilikoValueNew(2),
    SilikoValueNew(3),
    SilikoValueNew(5),
    SilikoValueNew(7),
    SilikoValueNew(11),
)

TestFunctionForEquality(FiveFloatsCanBeAdded,
    "add", 28.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0),
    SilikoValueNew(5.0),
    SilikoValueNew(7.0),
    SilikoValueNew(11.0),
)

TestFunctionForEquality(AddingMixedTypesGivesFloats,
    "add", 10.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0),
    SilikoValueNew(5.0),
)

TestFunctionForEquality(ThreeMinusTwoIsOne,
    "subtract", 1,
    SilikoValueNew(3),
    SilikoValueNew(2),
)

TestFunctionForEquality(ThreeMinusTwoIsOneWithFloats,
    "subtract", 1.0,
    SilikoValueNew(3.0),
    SilikoValueNew(2.0),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOne,
    "subtract", -1,
    SilikoValueNew(2),
    SilikoValueNew(3),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOneWithFloats,
    "subtract", -1.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0),
)

TestFunctionForEquality(SubtractingMixedNumbersGivesFloat,
    "subtract", 19.5,
    SilikoValueNew(23),
    SilikoValueNew(3.5),
)

TestFunctionForEquality(SubtractingMultipleIntegersDoesntCrash,
    "subtract", 4,
    SilikoValueNew(100),
    SilikoValueNew(50),
    SilikoValueNew(25),
    SilikoValueNew(12),
    SilikoValueNew(6),
    SilikoValueNew(3),
)

TestFunctionForEquality(SubtractingMultipleFloatsDoesntCrash,
    "subtract", 3.125,
    SilikoValueNew(100.0),
    SilikoValueNew(50.0),
    SilikoValueNew(25.0),
    SilikoValueNew(12.5),
    SilikoValueNew(6.25),
    SilikoValueNew(3.125),
)

TestFunctionForEquality(TwoTimesThreeIsSix,
    "multiply", 6,
    SilikoValueNew(2),
    SilikoValueNew(3)
)

TestFunctionForEquality(TwoTimesThreeIsSixWithFloats,
    "multiply", 6.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTen,
    "multiply", -10,
    SilikoValueNew(2),
    SilikoValueNew(-5)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTenWithFloats,
    "multiply", -10.0,
    SilikoValueNew(2.0),
    SilikoValueNew(-5.0),
)

TestFunctionForEquality(TwoWrongsMakeARight,
    "multiply", 70,
    SilikoValueNew(-7),
    SilikoValueNew(-10),
)

TestFunctionForEquality(TwoWrongsMakeARightWithFloats,
    "multiply", 70.0,
    SilikoValueNew(-7.0),
    SilikoValueNew(-10.0),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrash,
    "multiply", 60,
    SilikoValueNew(2),
    SilikoValueNew(3),
    SilikoValueNew(5),
    SilikoValueNew(2),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrashWithFloats,
    "multiply", 60.0,
    SilikoValueNew(2.0),
    SilikoValueNew(3.0),
    SilikoValueNew(5.0),
    SilikoValueNew(2.0),
)

TestFunctionForEquality(MultiplyingIntegerAndFloatIsFloat,
    "multiply", 30.0,
    SilikoValueNew(2),
    SilikoValueNew(15.0),
)

TestFunctionForEquality(TenDividedByTwoIsFive,
    "divide", 5,
    SilikoValueNew(10),
    SilikoValueNew(2)
)

TestFunctionForEquality(TenDividedByTwoIsFiveWithFloats,
    "divide", 5.0,
    SilikoValueNew(10.0),
    SilikoValueNew(2.0)
)

TestFunctionForEquality(MultipleDividesDontCrash,
    "divide", 1,
    SilikoValueNew(100),
    SilikoValueNew(25),
    SilikoValueNew(2),
    SilikoValueNew(2)
)

TestFunctionForEquality(MultipleDividesDontCrashWithFloats,
    "divide", 1.0,
    SilikoValueNew(100.0),
    SilikoValueNew(25.0),
    SilikoValueNew(2.0),
    SilikoValueNew(2.0)
)

TestFunctionForEquality(IntegersWillDivideToFloat,
    "divide", 2.5,
    SilikoValueNew(10),
    SilikoValueNew(4)
)

TestFunctionForEquality(DividingIntByFloatWorks,
    "divide", 5.0,
    SilikoValueNew(10),
    SilikoValueNew(2.0)
)

TestFunctionForEquality(TwoToTheTwoIsFour,
    "power", 4.0,
    SilikoValueNew(2),
    SilikoValueNew(2)
)

TestFunctionForEquality(OneHundredToTheHalfIsTen,
    "power", 10.0,
    SilikoValueNew(100.0),
    SilikoValueNew(0.5)
)
