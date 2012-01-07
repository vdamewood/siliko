#if !defined XVC_FUNCTIONS_H
#define XVC_FUNCTIONS_H
#include <setjmp.h>
#include "XvcStructs.h"

XvcNumber XvcFunctionCall(const char * name, int argc, XvcNumber * argv, jmp_buf jb);
#endif // XVC_FUNCTIONS_H