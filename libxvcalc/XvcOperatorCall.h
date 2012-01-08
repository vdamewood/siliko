#if !defined XVC_OPERATORS_H
#define XVC_OPERATORS_H

#include <setjmp.h>

#include "XvcStructs.h"

XvcNumber XvcOperatorCall(char Operator, XvcNumber LeftOperand, XvcNumber RightOperand, jmp_buf EscapePoint);
#endif // XVC_OPERATORS_H