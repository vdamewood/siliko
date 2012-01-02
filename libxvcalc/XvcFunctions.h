#if !defined FUNCTIONS_H
#define FUNCTIONS_H
#include <setjmp.h>
#include "structs.h"

number xvcalc_call_function(const char * name, int argc, number * argv, jmp_buf jb);
#endif /* FUNCTIONS_H */