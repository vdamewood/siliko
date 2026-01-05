#include <criterion/criterion.h>
#include <SilikoCore/Engine.h>
#include <math.h>
#include <float.h>

#define SUITE FunctionsTrigonometric
#define INSTALL_FUNCTION SilikoInstallMathFunctions

#include "Macros.h"
#include "FunctionMacros.h"

TestFunctionForEquality(SinZeroIsZero,
    "sin", 0.0,
    SilikoValueNew(0.0)
)

TestFunctionForEpsilon(SinNintyIsAbouOne,
    "sin", 1.0, DBL_EPSILON,
    SilikoValueNew(M_PI*0.5)
)

TestFunctionForEpsilon(SinOneEightyIsAbouZero,
    "sin", 0.0, DBL_EPSILON,
    SilikoValueNew(M_PI)
)

TestFunctionForEpsilon(SinTwoSeventyIsAboutNegativeOne,
    "sin", -1.0, DBL_EPSILON,
    SilikoValueNew(M_PI*1.5)
)

TestFunctionForEquality(CosZeroIsOne,
    "cos", 1.0,
    SilikoValueNew(0.0)
)

TestFunctionForEpsilon(CosNintyIsAboutZero,
    "cos", 0.0, DBL_EPSILON,
    SilikoValueNew(M_PI*0.5)
)

TestFunctionForEpsilon(CosOneEightyIsAboutNegativeOne,
    "cos", -1.0, DBL_EPSILON,
    SilikoValueNew(M_PI)
)

TestFunctionForEpsilon(CosTwoSeventyIsAboutZero,
    "cos", 0.0, DBL_EPSILON,
    SilikoValueNew(M_PI*1.5)
)

TestFunctionForEquality(TanZeroIsZero,
    "tan", 0.0,
    SilikoValueNew(0.0)
)

TestFunctionForEpsilon(TanFortyFiveIsAbouOne,
    "tan", 1.0, DBL_EPSILON,
    SilikoValueNew(M_PI*0.25)
)

TestFunctionForEpsilon(TanOneThirtyFiveIsAbouNegativeOne,
    "tan", -1.0, DBL_EPSILON,
    SilikoValueNew(M_PI*0.75)
)

TestFunctionForEpsilon(TanOneEightyIsAbouZero,
    "tan", 0.0, DBL_EPSILON,
    SilikoValueNew(M_PI)
)
