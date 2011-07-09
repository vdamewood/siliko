#if !defined FUNCTIONS_H
#define FUNCTIONS_H
#include <setjmp.h>
#include "structs.h"
typedef number (*function_ptr)(int, number *,jmp_buf);
function_ptr xvcalc_get_operator(char);
function_ptr xvcalc_get_function(char *);
#endif /* FUNCTIONS_H */