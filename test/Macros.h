#define SilikoValueNew(V) _Generic((V), \
    int:              SilikoValueNewInteger((long long int)V), \
    long long int:    SilikoValueNewInteger(V), \
    double:           SilikoValueNewReal(V) \
)
