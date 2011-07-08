#if !defined FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"
typedef number (*function_ptr)(int, number *);
function_ptr get_function(char *);
#endif /* FUNCTIONS_H */