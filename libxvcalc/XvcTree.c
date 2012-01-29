/*
 * Xvctree.c: Functions to manipulate abstract syntax trees.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "XvcTree.h"
#include "XvcArglist.h"
#include "XvcCleanup.h"
#include "XvcFunctionId.h"

XvcTree * XvcTreeNewOperator(XvcOperatorSymbol symbol, XvcTree * left, XvcTree * right)
{
	XvcTree * rVal;
	rVal = malloc(sizeof(XvcTree));
	rVal->op = malloc(sizeof(XvcOperator));
	rVal->op->args = malloc(sizeof(XvcNumber) * 2);
	XvcCleanupCacheTree(rVal);

	rVal->type = 'o';
	rVal->op->symbol = symbol;

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
	rVal->num->status = S_INTEGER;
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
	rVal->num->status = S_FLOAT;
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
