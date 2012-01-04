#if !defined XVC_ARGLIST_H
#define XVC_ARGLIST_H

#include "structs.h"

arglist * XvcArglistNew(tree * new_arg, arglist * old_list);
void XvcArglistDelete(arglist * in_arglist);
void XvcArglistDissolve(arglist * InArglist);
tree ** XvcArglistGetTrees(arglist * InArglist);

#endif // XVC_ARGLIST_H