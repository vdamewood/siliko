#if !defined SUITE
#error Must define SUITE to use FunctionMacros.h
#endif

#if !defined INSTALL_FUNCTION
#error Must define INSTALL_FUNCTION to use FunctionMacros.h
#endif

/*#define InitValue(VALUE) _Generic((VALUE), \
    signed char:            SilikoValueInitInteger, \
    short int:              SilikoValueInitInteger, \
    int:                    SilikoValueInitInteger, \
    long int:               SilikoValueInitInteger, \
    long long int:          SilikoValueInitInteger, \
    float:                  SilikoValueInitFloat,   \
    double:                 SilikoValueInitFloat,   \
    enum SilikoValueStatus: SilikoValueInitStatus,  \
    default:                SilikoValueInitInteger  \
)(VALUE)*/

#define TestFunction(NAME, FUNCTION, STATUS, RESULT_EXPRESSION, ...) \
Test(SUITE, NAME) \
{ \
    SilikoFunctionCaller *caller = SilikoFunctionCallerNew(); \
    cr_assert(caller != NULL); \
    INSTALL_FUNCTION(caller); \
    struct SilikoValue input[] = \
    { \
        __VA_ARGS__ \
    }; \
    struct SilikoValue result = SilikoFunctionCallerCall( \
        caller, \
        FUNCTION, \
        (sizeof input)/sizeof(struct SilikoValue), \
        input); \
    SilikoFunctionCallerDelete(caller); \
    cr_assert(result.Status == STATUS, \
        "Status: Expected: %d; Got %d;", STATUS, result.Status); \
    cr_assert(RESULT_EXPRESSION); \
}

#define TestFunctionForStatus(NAME, FUNCTION, STATUS, ...) \
    TestFunction(NAME, FUNCTION, \
        STATUS, \
        (result.Integer == 0), \
        __VA_ARGS__ \
)

#define TestFunctionForEpsilon(NAME, FUNCTION, RESULT, EPSILON, ...) \
    TestFunction(NAME, FUNCTION, \
        SILIKO_VAL_FLOAT, \
        (fabs(result.Float - RESULT) <= EPSILON), \
        __VA_ARGS__ \
)

#define TestFunctionForEquality(NAME, FUNCTION, RESULT, ...) \
    TestFunction(NAME, FUNCTION, \
        _Generic((RESULT), \
            signed char:            SILIKO_VAL_INTEGER, \
            short int:              SILIKO_VAL_INTEGER, \
            int:                    SILIKO_VAL_INTEGER, \
            long int:               SILIKO_VAL_INTEGER, \
            long long int:          SILIKO_VAL_INTEGER, \
            float:                  SILIKO_VAL_FLOAT,   \
            double:                 SILIKO_VAL_FLOAT,   \
            default:                SILIKO_VAL_INTEGER  \
        ), \
        (_Generic((RESULT), \
            signed char:            result.Integer, \
            short int:              result.Integer, \
            int:                    result.Integer, \
            long int:               result.Integer, \
            long long int:          result.Integer, \
            float:                  result.Float,   \
            double:                 result.Float,   \
            default:                result.Integer  \
        ) == RESULT), \
        __VA_ARGS__ \
)
