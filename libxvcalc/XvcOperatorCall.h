#if !defined XVC_OPERATORS_H
#define XVC_OPERATORS_H
#include <setjmp.h>
#include "XvcStructs.h"

#define  xvcalc_call_operator XvcOperatorCall
XvcNumber XvcOperatorCall(char operator, XvcNumber left, XvcNumber right, jmp_buf jb);
#endif // XVC_OPERATORS_H