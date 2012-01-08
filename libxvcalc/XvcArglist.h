#if !defined XVC_ARGLIST_H
#define XVC_ARGLIST_H

#include "XvcStructs.h"

XvcArglist * XvcArglistNew(XvcTree * NewArgument, XvcArglist * ListToExtend);
void XvcArglistDelete(XvcArglist * ArglistToDelete);
void XvcArglistDissolve(XvcArglist * ArglistToDissolve);
XvcTree ** XvcArglistGetTrees(XvcArglist * ArglistToExtract);

#endif // XVC_ARGLIST_H