#include <criterion/criterion.h>
#include <SilikoCore/FunctionCaller.h>

SilikoValue *GetFortyTwoInt(int ArgC, SilikoValue **ArgV)
{
    (void)ArgC;
    (void)ArgV;
    return SilikoValueNewInteger(42LL);
}

SilikoValue *GetFortyTwoFloat(int ArgC, SilikoValue **ArgV)
{
    (void)ArgC;
    (void)ArgV;
    return SilikoValueNewReal(42.0);
}

Test(FunctionCallerTests, NewCaller) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerDelete(Caller);
}

Test(FunctionCallerTests, GetIntFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerInstall(Caller, "gfti", GetFortyTwoInt);
    SilikoFunctionPointer TestValue = SilikoFunctionCallerGetFunction(Caller, "gfti");
    SilikoFunctionCallerDelete(Caller);
    cr_assert(TestValue == GetFortyTwoInt);
}

Test(FunctionCallerTests, GetFloatFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerInstall(Caller, "gftf", GetFortyTwoFloat);
    SilikoFunctionPointer TestValue = SilikoFunctionCallerGetFunction(Caller, "gftf");
    SilikoFunctionCallerDelete(Caller);
    cr_assert(TestValue == GetFortyTwoFloat);
}

Test(FunctionCallerTests, UseIntFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerInstall(Caller, "gfti", GetFortyTwoInt);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "gfti", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueInteger);
    cr_assert(SilikoValueToInteger(TestValue) == 42);
    SilikoValueDelete(TestValue);
}

Test(FunctionCallerTests, UseFloatFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerInstall(Caller, "gft", GetFortyTwoFloat);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "gft", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueReal);
    cr_assert(SilikoValueToReal(TestValue) == 42.0);
    SilikoValueDelete(TestValue);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoValue *TestValue = SilikoFunctionCallerCall(Caller, "bogus", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(SilikoValueGetStatus(TestValue) == SilikoValueError);
    cr_assert(SilikoValueToError(TestValue) == SilikoErrorFunctionName);
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
