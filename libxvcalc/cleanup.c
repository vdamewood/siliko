#include <stdlib.h>
#include <stdio.h>

#include "XvcEvaluate.h"
#include "cleanup.h"

typedef void (*del_fptr)(void*);

void debug_report(const char * function, void * address)
{
	if (!address) {
		printf("[NULL]:      %s\n", function);
	}
	else {
		printf("%p: %s\n", address, function);
	}
}

static void ** dangling_trees = NULL;
static void ** dangling_arglists = NULL;
static void ** dangling_ids = NULL;

static int tree_count = 0;
static int arglist_count = 0;
static int id_count = 0;

/* Scan through the list of items and save item. */
static void cache_item(void *** list, int * count, void * item)
{
	int i = 0;
	int item_is_saved = 0;
	void * relist = NULL;

	/* If the list given is empty, initialize it with a single empty space. */
	if (!(*list)) {
		if (!(*list = malloc(sizeof(void*)))) {
			/* FIXME. Fail gracefully. */
			fprintf(stderr, "libxvcalc: memory allocation failed for memory management!\n");
			exit(1);
		}
		/* Make this new element empty */
		**list = NULL;
		*count = 1;
	}
	
	/* Scan list for an empty (NULL) spot. If one is found, store the address there */
	for (i = 0; i < *count; i++) {
		if (!list[i]) {
			list[i] = item;
			item_is_saved = 1;
			break;
		}
	}
	
	/* if the item wasn't saved, it means the list was full. Realloc a new one. */
	if (!item_is_saved) {
		relist = realloc(*list, sizeof(void*) * ((*count) + 1));
		if (!relist) {
			fprintf(stderr, "libxvcalc: Memory reallocation failed.\n");
			exit(1);
		}
		*list = relist;
		(*list)[*count] = item;
		(*count)++;
	}
}

static void release_item(void *** list, int * count, void * item)
{
	int i;
	if (!item) { return; }
	for (i = 0; i < *count; i++) {
		if (list[i] == item) {
			list[i] = NULL;
		}
	}
}


static void delete_dangling_tree(void * dangling)
{
	//debug_report( __FUNCTION__, dangling);
	//xvcalc_delete_tree(dangling);
}

static void delete_dangling_arglist(void * dangling)
{
	//xvcalc_delete_arglist(dangling);
}

static void delete_dangling_id(void * dangling)
{
	//xvcalc_delete_id(dangling);
}

static void xvcalc_clear_dangling_items(void *** list, int *size, del_fptr delete)
{
	debug_report( __FUNCTION__, list);
	int i;
	for (i=0; i < *size; i++) {
		debug_report( __FUNCTION__, *list[i]);
		if (*list[i]) {
			delete((*list)[i]);
			(*list)[i] = NULL;
		}
	}
	free(*list);
	*list = NULL;
	*size = 0;
}



/* End Generics */

void XvcCleanup()
{
}

void xvcalc_cache_dangling_tree(tree * cache_tree)
{
	/*debug_report( __FUNCTION__, cache_tree);
	cache_item(&dangling_trees, &tree_count, cache_tree);*/
}

void xvcalc_release_dangling_tree(tree * release_tree)
{
	/*debug_report( __FUNCTION__, release_tree);
	release_item(&dangling_trees, &tree_count, release_tree);*/
}

void xvcalc_clear_dangling_trees()
{
	/*debug_report( __FUNCTION__, NULL);
	xvcalc_clear_dangling_items(&dangling_trees, &tree_count, delete_dangling_tree);*/
}

void xvcalc_cache_dangling_arglist(arglist * cache_arglist)
{
	/*debug_report( __FUNCTION__, cache_arglist);
	cache_item(&dangling_arglists, &arglist_count, cache_arglist);*/
}

void xvcalc_release_dangling_arglist(arglist * release_arglist)
{
	/*debug_report( __FUNCTION__, release_arglist);
	release_item(&dangling_arglists, &arglist_count, release_arglist);*/
}

void xvcalc_clear_dangling_arglists()
{
	/*debug_report( __FUNCTION__, NULL);
	xvcalc_clear_dangling_items(&dangling_arglists, &arglist_count, delete_dangling_arglist);*/
}

void xvcalc_cache_dangling_id(char * cache_id)
{
	/*debug_report( __FUNCTION__, cache_id);
	cache_item(&dangling_ids, &id_count, cache_id);*/
}

void xvcalc_release_dangling_id(char * release_id)
{
	/*debug_report( __FUNCTION__, release_id);
	release_item(&dangling_ids, &id_count, release_id);*/
}

void xvcalc_clear_dangling_ids()
{
	/*debug_report( __FUNCTION__, NULL);
	xvcalc_clear_dangling_items(&dangling_ids, &id_count, delete_dangling_id);*/
}
