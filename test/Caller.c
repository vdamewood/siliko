#include <criterion/criterion.h>
#include <SilikoCore/FunctionCaller.h>

struct SilikoValue GetFortyTwoInt(int ArgC, struct SilikoValue *ArgV)
{
    (void)ArgC;
    (void)ArgV;
    return SILIKO_VALUE(42LL);
}

struct SilikoValue GetFortyTwoFloat(int ArgC, struct SilikoValue *ArgV)
{
    (void)ArgC;
    (void)ArgV;
    return SILIKO_VALUE(42.0);
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
    struct SilikoValue TestValue = SilikoFunctionCallerCall(Caller, "gfti", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(TestValue.Status == SILIKO_VAL_INTEGER);
    cr_assert(TestValue.Integer == 42);
}

Test(FunctionCallerTests, UseFloatFunction) {
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    SilikoFunctionCallerInstall(Caller, "gft", GetFortyTwoFloat);
    struct SilikoValue TestValue = SilikoFunctionCallerCall(Caller, "gft", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(TestValue.Status == SILIKO_VAL_FLOAT);
    cr_assert(TestValue.Float == 42.0);
}

Test(FunctionCallerTests, HandleBadFunction)
{
    SilikoFunctionCaller *Caller = SilikoFunctionCallerNew();
    cr_assert(Caller != NULL);
    struct SilikoValue TestValue = SilikoFunctionCallerCall(Caller, "bogus", 0, NULL);
    SilikoFunctionCallerDelete(Caller);
    cr_assert(TestValue.Status == SILIKO_VAL_BAD_FUNCTION);
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
