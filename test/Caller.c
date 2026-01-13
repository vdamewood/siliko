#include <criterion/criterion.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Operator.h>
#include <SilikoCore/Math.h>

SilikoValue *GetFortyTwoInt(int argc, SilikoValue **argv)
{
    return SilikoValueCreateFromInteger(42LL);
}

SilikoValue *GetFortyTwoFloat(int argc, SilikoValue **argv)
{
    return SilikoValueCreateFromReal(42.0);
}

Test(EngineTests, NewCaller) {
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoEngineDestroy(Caller);
}

Test(EngineTests, GetIntFunction) {
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionCreate(GetFortyTwoInt);
    SilikoEngineInstallFunction(Caller, "gfti", TestFunction);
    SilikoFunction *ResultFunction = SilikoEngineFetchFunction(Caller, "gfti");
    cr_assert(TestFunction == ResultFunction);
    SilikoEngineDestroy(Caller);
}

Test(EngineTests, GetFloatFunction) {
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionCreate(GetFortyTwoFloat);
    SilikoEngineInstallFunction(Caller, "gftf", TestFunction);
    SilikoFunction *ResultFunction = SilikoEngineFetchFunction(Caller, "gftf");
    cr_assert(TestFunction == ResultFunction);
    SilikoEngineDestroy(Caller);
}

Test(EngineTests, UseIntFunction) {
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionCreate(GetFortyTwoInt);
    SilikoEngineInstallFunction(Caller, "gfti", TestFunction);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "gfti", 0, NULL);
    SilikoEngineDestroy(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueInteger);
    cr_assert(SilikoValueGetInteger(TestValue) == 42);
    SilikoValueDestroy(TestValue);
}

Test(EngineTests, UseFloatFunction) {
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionCreate(GetFortyTwoFloat);
    SilikoEngineInstallFunction(Caller, "gftf", TestFunction);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "gftf", 0, NULL);
    SilikoEngineDestroy(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueReal);
    cr_assert(SilikoValueGetReal(TestValue) == 42.0);
    SilikoValueDestroy(TestValue);
}

Test(EngineTests, HandleBadFunction)
{
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "bogus", 0, NULL);
    SilikoEngineDestroy(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueError);
    cr_assert(SilikoValueGetError(TestValue) == SilikoErrorFunctionName);
    SilikoValueDestroy(TestValue);
}

Test(EngineTests, FunctionsInstall)
{
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    int Result = SilikoInstallMathFunctions(Caller);
    SilikoEngineDestroy(Caller);
    cr_assert(Result);
}

Test(EngineTests, OperatorsInstall)
{
    SilikoEngine *Caller = SilikoEngineCreate();
    cr_assert(Caller != NULL);
    int Result = SilikoInstallOperators(Caller);
    SilikoEngineDestroy(Caller);
    cr_assert(Result);
}
