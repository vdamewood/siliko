#include <criterion/criterion.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Operator.h>
#include <SilikoCore/Math.h>

#define SUITE FunctionOperatorTests
#define INSTALL_FUNCTION SilikoInstallOperators

#include "Macros.h"
#include "FunctionMacros.h"

TestFunctionForEquality(TwoPlusThreeIsFive,
    "add", 5,
    SilikoValueCreate(2),
    SilikoValueCreate(3),
)

TestFunctionForEquality(TwoPlusThreeIsFiveWithFloats,
    "add", 5.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOne,
    "add", -1,
    SilikoValueCreate(2),
    SilikoValueCreate(-3),
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOneWithFloats,
    "add", -1.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(-3.0),
)

TestFunctionForEquality(OneEighthPlusHalfIsFiveEighths,
    "add", 0.625,
    SilikoValueCreate(0.125),
    SilikoValueCreate(0.5),
)

TestFunctionForEquality(FiveIntegerCanBeAdded,
    "add", 28,
    SilikoValueCreate(2),
    SilikoValueCreate(3),
    SilikoValueCreate(5),
    SilikoValueCreate(7),
    SilikoValueCreate(11),
)

TestFunctionForEquality(FiveFloatsCanBeAdded,
    "add", 28.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0),
    SilikoValueCreate(5.0),
    SilikoValueCreate(7.0),
    SilikoValueCreate(11.0),
)

TestFunctionForEquality(AddingMixedTypesGivesFloats,
    "add", 10.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0),
    SilikoValueCreate(5.0),
)

TestFunctionForEquality(ThreeMinusTwoIsOne,
    "subtract", 1,
    SilikoValueCreate(3),
    SilikoValueCreate(2),
)

TestFunctionForEquality(ThreeMinusTwoIsOneWithFloats,
    "subtract", 1.0,
    SilikoValueCreate(3.0),
    SilikoValueCreate(2.0),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOne,
    "subtract", -1,
    SilikoValueCreate(2),
    SilikoValueCreate(3),
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOneWithFloats,
    "subtract", -1.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0),
)

TestFunctionForEquality(SubtractingMixedNumbersGivesFloat,
    "subtract", 19.5,
    SilikoValueCreate(23),
    SilikoValueCreate(3.5),
)

TestFunctionForEquality(SubtractingMultipleIntegersDoesntCrash,
    "subtract", 4,
    SilikoValueCreate(100),
    SilikoValueCreate(50),
    SilikoValueCreate(25),
    SilikoValueCreate(12),
    SilikoValueCreate(6),
    SilikoValueCreate(3),
)

TestFunctionForEquality(SubtractingMultipleFloatsDoesntCrash,
    "subtract", 3.125,
    SilikoValueCreate(100.0),
    SilikoValueCreate(50.0),
    SilikoValueCreate(25.0),
    SilikoValueCreate(12.5),
    SilikoValueCreate(6.25),
    SilikoValueCreate(3.125),
)

TestFunctionForEquality(TwoTimesThreeIsSix,
    "multiply", 6,
    SilikoValueCreate(2),
    SilikoValueCreate(3)
)

TestFunctionForEquality(TwoTimesThreeIsSixWithFloats,
    "multiply", 6.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTen,
    "multiply", -10,
    SilikoValueCreate(2),
    SilikoValueCreate(-5)
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTenWithFloats,
    "multiply", -10.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(-5.0),
)

TestFunctionForEquality(TwoWrongsMakeARight,
    "multiply", 70,
    SilikoValueCreate(-7),
    SilikoValueCreate(-10),
)

TestFunctionForEquality(TwoWrongsMakeARightWithFloats,
    "multiply", 70.0,
    SilikoValueCreate(-7.0),
    SilikoValueCreate(-10.0),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrash,
    "multiply", 60,
    SilikoValueCreate(2),
    SilikoValueCreate(3),
    SilikoValueCreate(5),
    SilikoValueCreate(2),
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrashWithFloats,
    "multiply", 60.0,
    SilikoValueCreate(2.0),
    SilikoValueCreate(3.0),
    SilikoValueCreate(5.0),
    SilikoValueCreate(2.0),
)

TestFunctionForEquality(MultiplyingIntegerAndFloatIsFloat,
    "multiply", 30.0,
    SilikoValueCreate(2),
    SilikoValueCreate(15.0),
)

TestFunctionForEquality(TenDividedByTwoIsFive,
    "divide", 5,
    SilikoValueCreate(10),
    SilikoValueCreate(2)
)

TestFunctionForEquality(TenDividedByTwoIsFiveWithFloats,
    "divide", 5.0,
    SilikoValueCreate(10.0),
    SilikoValueCreate(2.0)
)

TestFunctionForEquality(MultipleDividesDontCrash,
    "divide", 1,
    SilikoValueCreate(100),
    SilikoValueCreate(25),
    SilikoValueCreate(2),
    SilikoValueCreate(2)
)

TestFunctionForEquality(MultipleDividesDontCrashWithFloats,
    "divide", 1.0,
    SilikoValueCreate(100.0),
    SilikoValueCreate(25.0),
    SilikoValueCreate(2.0),
    SilikoValueCreate(2.0)
)

TestFunctionForEquality(IntegersWillDivideToFloat,
    "divide", 2.5,
    SilikoValueCreate(10),
    SilikoValueCreate(4)
)

TestFunctionForEquality(DividingIntByFloatWorks,
    "divide", 5.0,
    SilikoValueCreate(10),
    SilikoValueCreate(2.0)
)

TestFunctionForEquality(TwoToTheTwoIsFour,
    "power", 4.0,
    SilikoValueCreate(2),
    SilikoValueCreate(2)
)

TestFunctionForEquality(OneHundredToTheHalfIsTen,
    "power", 10.0,
    SilikoValueCreate(100.0),
    SilikoValueCreate(0.5)
)
