#include <stdlib.h>

#include "cleanup.h"

/* Functions to clean up after a syntax error. */

struct xvcalc_treelist;
typedef struct xvcalc_treelist treelist;

struct xvcalc_treelist {
	treelist * parent;
	treelist * child;
	tree * tree;
};

static tree * dangling_trees = NULL;
static arglist * dangling_arglist = NULL;
static char * dangling_id = NULL;

void add_dangling_tree(treelist * list, tree * in_tree);
treelist * find_tree(treelist * in_list, tree * in_tree);
void xvcalc_memi_delete_trees(treelist * list);
void xvcalc_memi_delete_arglist(arglist * in_arglist);
void xvcalc_mem_clear_id();
void new_dangling_tree(tree * in_tree);

/* ************************************************************************* */
/* ************************************************************************* */

void xvcalc_mem_append_tree(tree * in_tree)
{
	/*printf("In function %s\n", __FUNCTION__);
	if(dangling_trees) add_dangling_tree(dangling_trees, in_tree);
	else new_dangling_tree(in_tree);*/
}

/*void new_dangling_tree(tree * in_tree)
{
	printf("In function %s\n", __FUNCTION__);
	dangling_trees = malloc(sizeof(treelist));
	printf("T: %p\tL: %p\n", in_tree, dangling_trees);
	dangling_trees->parent = NULL;
	dangling_trees->child = NULL;
	dangling_trees->tree = in_tree;
}*/

/*void add_dangling_tree(treelist * list, tree * in_tree)
{
	printf("In function %s\n", __FUNCTION__);
	if(list->child) {
		add_dangling_tree(list->child, in_tree);
	}
	else {
		list->child = malloc(sizeof(treelist));
		list->child->parent = list;
		list->child->child = NULL;
		list->child->tree = in_tree;
		printf("T: %p\tL: %p\n", in_tree, list->child);
	}
}*/

void xvcalc_mem_remove_tree(tree * dead_tree)
{
	/*printf("In function %s\n", __FUNCTION__);
	treelist * node;
	node = find_tree(dangling_trees, dead_tree);
	printf("T: %p\tL: %p\n", dead_tree, node);
	if(node) {
		if (node->parent) {
			node->parent->child = node->child;
			if (node->child) {
				node->child->parent = node->parent;
			}
		}
		else {
			dangling_trees = node->child;
			if (node->child) {
				node->child->parent = NULL;
			}
		}
	}*/
}

/*treelist * find_tree(treelist * in_list, tree * in_tree)
{
	printf("In function %s\n", __FUNCTION__);
	if (!in_list) return NULL;
	else if (in_list->tree == in_tree) return in_list;
	return find_tree(in_list->child, in_tree);
}*/

void xvcalc_mem_delete_trees()
{
	/*printf("In function %s\n", __FUNCTION__);
	xvcalc_memi_delete_trees(dangling_trees);*/
}

/*void xvcalc_memi_delete_trees(treelist * list)
{
	printf("In function %s\n", __FUNCTION__);
	if (list->child) xvcalc_memi_delete_trees(list->child);
	xvcalc_delete_tree(list->tree);
}*/

/* ************************************************************************* */
/* ************************************************************************* */


void xvcalc_mem_set_arglist(arglist * new_arglist)
{
	/*printf("In function %s\n", __FUNCTION__);
	dangling_arglist = new_arglist;*/
}

void xvcalc_mem_delete_arglist()
{
	/*printf("In function %s\n", __FUNCTION__);
	xvcalc_memi_delete_arglist(dangling_arglist);
	xvcalc_mem_clear_arglist();*/
}

void xvcalc_mem_clear_arglist()
{
	/*printf("In function %s\n", __FUNCTION__);
	dangling_arglist = NULL;*/
}

/*void xvcalc_memi_delete_arglist(arglist * in_arglist)
{
	printf("In function %s\n", __FUNCTION__);
	if (in_arglist->next) xvcalc_mem_delete_arglist(in_arglist->next);
	xvcalc_delete_tree(in_arglist->value);
	free(in_arglist);
}*/

/* ************************************************************************* */
/* ************************************************************************* */


void xvcalc_mem_set_id(char * new_id)
{
	/*printf("In function %s\n", __FUNCTION__);
	dangling_id = new_id;*/
}

void xvcalc_mem_delete_id()
{
	/*printf("In function %s\n", __FUNCTION__);
	free(dangling_id);
	xvcalc_mem_clear_id();*/
}

void xvcalc_mem_clear_id()
{
	/*printf("In function %s\n", __FUNCTION__);
	dangling_id = NULL;*/
}
