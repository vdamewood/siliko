#if !defined XVC_FUNCTIONS_H
#define XVC_FUNCTIONS_H

#include <setjmp.h>

#include "XvcStructs.h"

XvcNumber XvcFunctionCall(const char * FunctionName, int ArgumentCount, XvcNumber * Arguments, jmp_buf EscapePoint);
#endif // XVC_FUNCTIONS_H
