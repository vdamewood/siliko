#if !defined XVC_TREE_H
#define XVC_TREE_H

#include "XvcStructs.h"

XvcTree * XvcTreeNewOperator(char Operator, XvcTree * LeftOperand, XvcTree * RightOperand);
XvcTree * XvcTreeNewInteger(int Value);
XvcTree * XvcTreeNewFloat(float Value);
XvcTree * XvcTreeNewFunction(char * FunctionName, XvcArglist * FunctionArguments);
void XvcTreeDelete(XvcTree * TreeToDelete);

#endif // XVC_TREE_H
