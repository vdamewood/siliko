#include "structs.h"

void xvcalc_mem_cache_dangling_tree(tree *);
void xvcalc_mem_release_dangling_tree(tree *);
void xvcalc_mem_clear_dangling_trees();

void xvcalc_mem_cache_dangling_arglist(arglist *);
void xvcalc_mem_release_dangling_arglist(arglist *);
void xvcalc_mem_clear_dangling_arglists();

void xvcalc_mem_cache_dangling_id(char *);
void xvcalc_mem_release_dangling_id(char *);
void xvcalc_mem_clear_dangling_ids();
