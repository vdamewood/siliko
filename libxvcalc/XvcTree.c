#include <stdlib.h>

#include "XvcTree.h"
#include "XvcArglist.h"
#include "XvcCleanup.h"
#include "XvcFunctionId.h"

XvcTree * XvcTreeNewOperator(char type, XvcTree * left, XvcTree * right)
{
	XvcTree * rVal;
	rVal = malloc(sizeof(XvcTree));
	rVal->op = malloc(sizeof(XvcOperator));
	rVal->op->args = malloc(sizeof(XvcNumber) * 2);
	XvcCleanupCacheTree(rVal);

	rVal->type = 'o';
	rVal->op->type = type;

	rVal->op->left = left;
	XvcCleanupReleaseTree(left);

	rVal->op->right = right;
	XvcCleanupReleaseTree(right);
	return rVal;
}

XvcTree * XvcTreeNewInteger(int value)
{
	XvcTree * rVal;
	rVal = malloc(sizeof(XvcTree));
	rVal->num = malloc(sizeof(XvcNumber));
	XvcCleanupCacheTree(rVal);

	rVal->type = 'n';
	rVal->num->type = 'i';
	rVal->num->i = value;
	return rVal;
}

XvcTree * XvcTreeNewFloat(float value)
{
	XvcTree * rVal;
	rVal = malloc(sizeof(XvcTree));
	rVal->num = malloc(sizeof(XvcNumber));
	XvcCleanupCacheTree(rVal);
	
	rVal->type = 'n';
	rVal->num->type = 'f';
	rVal->num->f = value;
	return rVal;
}

XvcTree * XvcTreeNewFunction(char * name, XvcArglist * in_arglist)
{
	XvcTree * rVal;
	rVal = malloc(sizeof(XvcTree));
	rVal->func = malloc(sizeof(XvcFunction));
	XvcCleanupCacheTree(rVal);
	
	rVal->type = 'f';
	rVal->func->name = name;
	XvcCleanupReleaseFunctionId(name);
	
	if(in_arglist) {
		rVal->func->arg_count = in_arglist->depth;

		rVal->func->arg_vector = XvcArglistGetTrees(in_arglist);
		XvcCleanupReleaseArglist(in_arglist);
		XvcArglistDissolve(in_arglist);		
	}
	else {
		rVal->func->arg_count = 0;
		rVal->func->arg_vector = NULL;
	}
	
	return rVal;
}

void XvcTreeDelete(XvcTree * tree)
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
				free(tree->func);
				break;
		}
		free(tree);
	}
}
