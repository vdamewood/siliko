#include <stdlib.h>

#include "cleanup.h"

void debug_report(const char * function, void * address)
{
	if (!address) {
		printf("[NULL]:      %s\n", function);
	}
	else {
		printf("%p: %s\n", address, function);
	}
}

void xvcalc_cache_dangling_tree(tree * cache_tree)
{
	debug_report( __FUNCTION__, cache_tree);
}

void xvcalc_release_dangling_tree(tree * release_tree)
{
	debug_report( __FUNCTION__, release_tree);	
}

void xvcalc_clear_dangling_trees()
{
	debug_report( __FUNCTION__, NULL);
}

void xvcalc_cache_dangling_arglist(arglist * cache_arglist)
{
	debug_report( __FUNCTION__, cache_arglist);
}

void xvcalc_release_dangling_arglist(arglist * release_arglist)
{
	debug_report( __FUNCTION__, release_arglist);
}

void xvcalc_clear_dangling_arglists()
{
	debug_report( __FUNCTION__, NULL);
}

void xvcalc_cache_dangling_id(char * cache_id)
{
}

void xvcalc_release_dangling_id(char * release_id)
{
}

void xvcalc_clear_dangling_ids()
{
}
