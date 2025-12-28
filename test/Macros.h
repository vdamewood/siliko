#define SilikoValueNew(V) _Generic((V), \
    int:              SilikoValueNewFromInteger((long long int)V), \
    long long int:    SilikoValueNewFromInteger(V), \
    double:           SilikoValueNewFromReal(V) \
)
