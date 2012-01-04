#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#include "XvcTree.h"
#include "xvcalc.h"
#include "tree.h"
#include "XvcFunctions.h"
#include "XvcOperators.h"
#include "xvcalcix.h"
#include "cleanup.h"

arglist * XvcArglistNew(tree * new_arg, arglist * old_list)
{
	arglist * rVal;
	rVal = malloc(sizeof(arglist));
	xvcalc_cache_dangling_arglist(rVal);
	if (old_list) rVal->depth = old_list->depth + 1;
	else rVal->depth = 1;
	rVal->value = new_arg;
	rVal->next = old_list;
	xvcalc_release_dangling_tree(new_arg);
	xvcalc_release_dangling_arglist(old_list);
	return rVal;
}

void XvcArglistDelete(arglist * in_arglist)
{
	if (in_arglist) {
		XvcArglistDelete(in_arglist->next);
		XvcTreeDelete(in_arglist->value);
		free(in_arglist);
	 }
}

// Destroy the arglist, but leave the trees contained therein intact.
void XvcArglistDissolve(arglist * InArglist)
{
	if (InArglist && InArglist->next) XvcArglistDissolve(InArglist->next);
	free(InArglist);
}

tree ** XvcArglistGetTrees(arglist * InArglist)
{
	tree ** rVal = malloc(InArglist->depth * sizeof(tree*));
	arglist * current = InArglist;
	int i;
	
	if (rVal) {
		for (i = 0; i < InArglist->depth; i++) {
			rVal[i] = current->value;
			current = current->next;
		}
	}
}


// Deprecated
void xvcalc_arglist_to_array(tree ** array, arglist * in_arglist)
{
	*array = in_arglist->value;
	if (in_arglist->next)
		xvcalc_arglist_to_array(array+1, in_arglist->next);
	free(in_arglist);
}
