#if !defined XvcOperators_H
#define XvcOperators_H
#include <setjmp.h>
#include "structs.h"

number xvcalc_call_operator(char operator, number * operands, jmp_buf jb);
#endif /* XvcOperators_H */