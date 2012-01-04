#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#include "xvcalc.h"
#include "tree.h"
#include "XvcFunctions.h"
#include "XvcOperators.h"
#include "xvcalcix.h"
#include "cleanup.h"

#define XvcTreeNewOperator  xvcalc_new_operation
#define XvcTreeNewInteger   xvcalc_new_int
#define XvcTreeNewFloat     xvcalc_new_float
#define XvcTreeNewFunction  xvcalc_new_function
#define XvcTreeDelete       xvcalc_delete_tree

tree * XvcTreeNewOperator(char type, tree * left, tree * right)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->op = malloc(sizeof(operation));
	rVal->op->args = malloc(sizeof(number) * 2);
	xvcalc_cache_dangling_tree(rVal);

	rVal->type = 'o';
	rVal->op->type = type;

	rVal->op->left = left;
	xvcalc_release_dangling_tree(left);

	rVal->op->right = right;
	xvcalc_release_dangling_tree(right);
	return rVal;
}

tree * XvcTreeNewInteger(int value)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->num = malloc(sizeof(number));
	xvcalc_cache_dangling_tree(rVal);

	rVal->type = 'n';
	rVal->num->type = 'i';
	rVal->num->i = value;
	return rVal;
}

tree * XvcTreeNewFloat(float value)
{
	struct xvcalc_tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->num = malloc(sizeof(number));
	xvcalc_cache_dangling_tree(rVal);
	
	rVal->type = 'n';
	rVal->num->type = 'f';
	rVal->num->f = value;
	return rVal;
}

tree * XvcTreeNewFunction(char * name, arglist * in_arglist)
{
	tree * rVal;
	rVal = malloc(sizeof(tree));
	rVal->func = malloc(sizeof(function));
	xvcalc_cache_dangling_tree(rVal);
	
	rVal->type = 'f';
	rVal->func->name = name;
	xvcalc_release_dangling_id(name);
	
	if(in_arglist) {
		rVal->func->arg_count = in_arglist->depth;
		rVal->func->arg_vector = malloc(sizeof(tree *) * in_arglist->depth);
		/* FIXME: Make arglist_to_array non-descructive on the arglist. That
		   way the trees in the arglist can be released here instead of in
		   arglist to array */
		xvcalc_arglist_to_array(rVal->func->arg_vector, in_arglist);
		xvcalc_release_dangling_arglist(in_arglist);
		rVal->func->eval_args = malloc(sizeof(number) * rVal->func->arg_count);
	}
	else {
		rVal->func->arg_count = 0;
		rVal->func->arg_vector = NULL;
		rVal->func->eval_args = NULL;
	}
	
	return rVal;
}

void XvcTreeDelete(tree * tree)
{
	int i;
	if (tree) {
		switch (tree->type) {
			case 'n':
				free(tree->num);
				break;
			case 'o':
				xvcalc_delete_tree(tree->op->left);
				xvcalc_delete_tree(tree->op->right);
				free(tree->op->args);
				free(tree->op);
				break;
			case 'f':
				free(tree->func->name);
				for (i = 0; i <  tree->func->arg_count; i++) {
					xvcalc_delete_tree(
						tree->func->arg_vector[i]);
				}
				free(tree->func->arg_vector);
				free(tree->func->eval_args);
				free(tree->func);
				break;
		}
		free(tree);
	}
}
