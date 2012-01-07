#if !defined XVC_CLEANUP_H
#define XVC_CLEANUP_H
#include "XvcStructs.h"


void XvcMemoryCleanup();
void xvcalc_cache_dangling_tree(tree * in);
void xvcalc_release_dangling_tree(tree * in);
void xvcalc_clear_dangling_trees();
void xvcalc_cache_dangling_arglist(arglist * in);
void xvcalc_release_dangling_arglist(arglist * in);
void xvcalc_clear_dangling_arglists();
void xvcalc_cache_dangling_id(char * in);
void xvcalc_release_dangling_id(char * in);
void xvcalc_clear_dangling_ids();

#define XvcCleanupClearAll    XvcMemoryCleanup

#define XvcCleanupCacheArglist      xvcalc_cache_dangling_arglist
#define XvcCleanupReleaseArglist    xvcalc_release_dangling_arglist
#define XvcCleanupClearArglists     xvcalc_clear_dangling_arglists
#define XvcCleanupCacheTree         xvcalc_cache_dangling_tree
#define XvcCleanupReleaseTree       xvcalc_release_dangling_tree
#define XvcCleanupClearTrees        xvcalc_clear_dangling_trees
#define XvcCleanupCacheFunctionId   xvcalc_cache_dangling_id
#define XvcCleanupReleaseFunctionId xvcalc_release_dangling_id
#define XvcCleanupClearFunctionIds  xvcalc_clear_dangling_id

#endif // XVC_CLEANUP_H