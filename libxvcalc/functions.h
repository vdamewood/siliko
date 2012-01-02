#if !defined FUNCTIONS_H
#define FUNCTIONS_H
#include <setjmp.h>
#include "structs.h"

number xvcalc_call_operator(char operator, number * operands, jmp_buf jb);
number xvcalc_call_function(const char * name, int argc, number * args, jmp_buf jb);
#endif /* FUNCTIONS_H */