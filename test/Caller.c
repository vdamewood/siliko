#include <criterion/criterion.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Operator.h>
#include <SilikoCore/Math.h>

SilikoValue *GetFortyTwoInt(int argc, SilikoValue **argv)
{
    return SilikoValueNewFromInteger(42LL);
}

SilikoValue *GetFortyTwoFloat(int argc, SilikoValue **argv)
{
    return SilikoValueNewFromReal(42.0);
}

Test(EngineTests, NewCaller) {
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoEngineDelete(Caller);
}

Test(EngineTests, GetIntFunction) {
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoInt);
    SilikoEngineInstallFunction(Caller, "gfti", TestFunction);
    SilikoFunction *ResultFunction = SilikoEngineFetchFunction(Caller, "gfti");
    cr_assert(TestFunction == ResultFunction);
    SilikoEngineDelete(Caller);
}

Test(EngineTests, GetFloatFunction) {
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoFloat);
    SilikoEngineInstallFunction(Caller, "gftf", TestFunction);
    SilikoFunction *ResultFunction = SilikoEngineFetchFunction(Caller, "gftf");
    cr_assert(TestFunction == ResultFunction);
    SilikoEngineDelete(Caller);
}

Test(EngineTests, UseIntFunction) {
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoInt);
    SilikoEngineInstallFunction(Caller, "gfti", TestFunction);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "gfti", 0, NULL);
    SilikoEngineDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueInteger);
    cr_assert(SilikoValueGetInteger(TestValue) == 42);
    SilikoValueDelete(TestValue);
}

Test(EngineTests, UseFloatFunction) {
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoFloat);
    SilikoEngineInstallFunction(Caller, "gftf", TestFunction);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "gftf", 0, NULL);
    SilikoEngineDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueReal);
    cr_assert(SilikoValueGetReal(TestValue) == 42.0);
    SilikoValueDelete(TestValue);
}

Test(EngineTests, HandleBadFunction)
{
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    SilikoValue *TestValue = SilikoEngineCallFunction(Caller, "bogus", 0, NULL);
    SilikoEngineDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueError);
    cr_assert(SilikoValueGetError(TestValue) == SilikoErrorFunctionName);
    SilikoValueDelete(TestValue);
}

Test(EngineTests, FunctionsInstall)
{
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    int Result = SilikoInstallMathFunctions(Caller);
    SilikoEngineDelete(Caller);
    cr_assert(Result);
}

Test(EngineTests, OperatorsInstall)
{
    SilikoEngine *Caller = SilikoEngineNew();
    cr_assert(Caller != NULL);
    int Result = SilikoInstallOperators(Caller);
    SilikoEngineDelete(Caller);
    cr_assert(Result);
}
