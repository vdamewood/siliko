#if !defined XVC_TREE_H
#define XVC_TREE_H

#include "structs.h"

tree * XvcTreeNewOperator(char type, tree * left, tree * right);
tree * XvcTreeNewInteger(int value);
tree * XvcTreeNewFloat(float value);
tree * XvcTreeNewFunction(char * name, arglist * in_arglist);
void XvcTreeDelete(tree * tree);

#endif // XVC_TREE_H
