#if !defined XVC_CLEANUP_H
#define XVC_CLEANUP_H

#include "XvcStructs.h"

void XvcCleanupCacheTree(XvcTree * TreeToCache);
void XvcCleanupReleaseTree(XvcTree * TreeToRelease);
void XvcCleanupClearTrees();
void XvcCleanupCacheArglist(XvcArglist * ArglistToCache);
void XvcCleanupReleaseArglist(XvcArglist * ArglistToRelease);
void XvcCleanupClearArglists();
void XvcCleanupCacheFunctionId(char * FunctionIdToCache);
void XvcCleanupReleaseFunctionId(char * FunctionIdToRelease);
void XvcCleanupClearFunctionIds();
void XvcCleanupClearAll();

#endif // XVC_CLEANUP_H