#define SilikoValueCreate(V) _Generic((V), \
    int:              SilikoValueCreateFromInteger((long long int)V), \
    long long int:    SilikoValueCreateFromInteger(V), \
    double:           SilikoValueCreateFromReal(V) \
)
