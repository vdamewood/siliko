#include <criterion/criterion.h>
#include <SilikoCore/FunctionCaller.h>

SilikoValue *GetFortyTwoInt(int argc, SilikoValue **argv)
{
    return SilikoValueNewFromInteger(42LL);
}

SilikoValue *GetFortyTwoFloat(int argc, SilikoValue **argv)
{
    return SilikoValueNewFromReal(42.0);
}

Test(FunctionCallerTests, NewCaller) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerDelete(Caller);
}

Test(FunctionCallerTests, GetIntFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoInt);
    SilikoFunctionCallerInstall(Caller, "gfti", TestFunction);
    SilikoFunction *ResultFunction = SilikoFunctionCallerGetFunction(Caller, "gfti");
    cr_assert(TestFunction == ResultFunction);
    SilikoFunctionCallerDelete(Caller);
}

Test(FunctionCallerTests, GetFloatFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoFloat);
    SilikoFunctionCallerInstall(Caller, "gftf", TestFunction);
    SilikoFunction *ResultFunction = SilikoFunctionCallerGetFunction(Caller, "gftf");
    cr_assert(TestFunction == ResultFunction);
    SilikoFunctionCallerDelete(Caller);
}

Test(FunctionCallerTests, UseIntFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoInt);
    SilikoFunctionCallerInstall(Caller, "gfti", TestFunction);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "gfti", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueInteger);
    cr_assert(SilikoValueGetInteger(TestValue) == 42);
    SilikoValueDelete(TestValue);
}

Test(FunctionCallerTests, UseFloatFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunction *TestFunction = SilikoPureFunctionNew(GetFortyTwoFloat);
    SilikoFunctionCallerInstall(Caller, "gftf", TestFunction);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "gftf", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueReal);
    cr_assert(SilikoValueGetReal(TestValue) == 42.0);
    SilikoValueDelete(TestValue);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "bogus", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueError);
    cr_assert(SilikoValueGetError(TestValue) == SilikoErrorFunctionName);
    SilikoValueDelete(TestValue);
}

Test(FunctionCallerTests, FunctionsInstall)
{
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    int Result = SilikoFunctionCallerInstallFunctions(Caller);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(Result);
}

Test(FunctionCallerTests, OperatorsInstall)
{
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    int Result = SilikoFunctionCallerInstallOperators(Caller);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(Result);
}
