#if !defined SUITE
#error Must define SUITE to use FunctionMacros.h
#endif

#if !defined INSTALL_FUNCTION
#error Must define INSTALL_FUNCTION to use FunctionMacros.h
#endif

#define TestFunction(NAME, FUNCTION, STATUS, RESULT_EXPRESSION, ...) \
Test(SUITE, NAME) \
{ \
    SilikoFunctionCaller *caller = SilikoFunctionCallerNew(); \
    cr_assert(caller != NULL); \
    INSTALL_FUNCTION(caller); \
    SilikoValue *input[] = \
    { \
        __VA_ARGS__ \
    }; \
    SilikoValue *result = SilikoFunctionCallerCall( \
        caller, \
        FUNCTION, \
        (sizeof input)/sizeof(input[0]), \
        input); \
    SilikoFunctionCallerDelete(caller); \
    cr_assert(SilikoValueGetStatus(result) == STATUS, \
        "Status: Expected: %d; Got %d;", STATUS, SilikoValueGetStatus(result)); \
    cr_assert(RESULT_EXPRESSION); \
}

#define TestFunctionForStatus(NAME, FUNCTION, STATUS, ...) \
    TestFunction(NAME, FUNCTION, \
        STATUS, \
        (SilikoValueToInteger(result) == 0), \
        __VA_ARGS__ \
)

#define TestFunctionForEpsilon(NAME, FUNCTION, RESULT, EPSILON, ...) \
    TestFunction(NAME, FUNCTION, \
        SilikoValueReal, \
        (fabs(SilikoValueToReal(result) - RESULT) <= EPSILON), \
        __VA_ARGS__ \
)

#define TestFunctionForEquality(NAME, FUNCTION, RESULT, ...) \
    TestFunction(NAME, FUNCTION, \
        _Generic((RESULT), \
            signed char:            SilikoValueInteger, \
            short int:              SilikoValueInteger, \
            int:                    SilikoValueInteger, \
            long int:               SilikoValueInteger, \
            long long int:          SilikoValueInteger, \
            float:                  SilikoValueReal,   \
            double:                 SilikoValueReal,   \
            default:                SilikoValueInteger  \
        ), \
        (_Generic((RESULT), \
            signed char:            SilikoValueToInteger(result), \
            short int:              SilikoValueToInteger(result), \
            int:                    SilikoValueToInteger(result), \
            long int:               SilikoValueToInteger(result), \
            long long int:          SilikoValueToInteger(result), \
            float:                  SilikoValueToReal(result),   \
            double:                 SilikoValueToReal(result),   \
            default:                SilikoValueToInteger(result)  \
        ) == RESULT), \
        __VA_ARGS__ \
)
