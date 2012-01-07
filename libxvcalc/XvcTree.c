#include <stdlib.h>

#include "XvcArglist.h"
#include "XvcCleanup.h"

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
		rVal->func->eval_args = malloc(sizeof(number) * rVal->func->arg_count);

		rVal->func->arg_vector = XvcArglistGetTrees(in_arglist);
		xvcalc_release_dangling_arglist(in_arglist);
		XvcArglistDissolve(in_arglist);		
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
				XvcTreeDelete(tree->op->left);
				XvcTreeDelete(tree->op->right);
				free(tree->op->args);
				free(tree->op);
				break;
			case 'f':
				XvcFunctionIdDelete(tree->func->name);
				for (i = 0; i <  tree->func->arg_count; i++) {
					XvcTreeDelete(tree->func->arg_vector[i]);
				}
				free(tree->func->arg_vector);
				free(tree->func->eval_args);
				free(tree->func);
				break;
		}
		free(tree);
	}
}
