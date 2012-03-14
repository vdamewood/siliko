/*
 * Xavitree.c: Functions to manipulate abstract syntax trees.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviCleanup.h"
#include "XaviFunctionId.h"

XaviTree * XaviTreeNewOperator(XaviOperatorSymbol symbol, XaviTree * left, XaviTree * right, XaviMemoryPool * pool)
{
	XaviTree * rVal;
	XaviOperator * rValOp;
	XaviNumber * rValArgs;

	rVal = malloc(sizeof(XaviTree)); //if
	rValOp = malloc(sizeof(XaviOperator)); //if
	rValArgs = malloc(sizeof(XaviNumber) * 2); //if

	if (!rVal || !rValOp || !rValArgs) {
		free(rVal);
		free(rValOp);
		free(rValArgs);
		return NULL;
	}
	
	rVal->op = rValOp;
	rVal->op->args = rValArgs;
	
	
	XaviCleanupCacheTree(rVal, pool);

	rVal->type = 'o';
	rVal->op->symbol = symbol;

	rVal->op->left = left;
	XaviCleanupReleaseTree(left, pool);

	rVal->op->right = right;
	XaviCleanupReleaseTree(right, pool);
	return rVal;
}

XaviTree * XaviTreeNewInteger(int value, XaviMemoryPool * pool)
{
	XaviTree * rVal;
	XaviNumber * rValNum;

	rVal = malloc(sizeof(XaviTree)); //if
	rValNum = malloc(sizeof(XaviNumber)); //if

	if (!rVal || !rValNum) {
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	XaviCleanupCacheTree(rVal, pool);

	rVal->type = 'n';
	rVal->num->status = S_INTEGER;
	rVal->num->i = value;
	return rVal;
}

XaviTree * XaviTreeNewFloat(float value, XaviMemoryPool * pool)
{
	XaviTree * rVal;
	XaviNumber * rValNum;
	
	rVal = malloc(sizeof(XaviTree)); //if
	rValNum = malloc(sizeof(XaviNumber)); //if
	
	if (!rVal || !rValNum) {
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	XaviCleanupCacheTree(rVal, pool);
	
	rVal->type = 'n';
	rVal->num->status = S_FLOAT;
	rVal->num->f = value;
	return rVal;
}

XaviTree * XaviTreeNewFunction(char * name, XaviArglist * in_arglist, XaviMemoryPool * pool)
{
	XaviTree * rVal;
	XaviFunction * rValFunc;
	
	rVal = malloc(sizeof(XaviTree)); //if
	rValFunc = malloc(sizeof(XaviFunction)); //if
	
	if (!rVal || !rValFunc) {
		free(rVal);
		free(rValFunc);
		return NULL;
	}
	rVal->func = rValFunc;

	XaviCleanupCacheTree(rVal, pool);
	
	rVal->type = 'f';
	rVal->func->name = name;
	XaviCleanupReleaseFunctionId(name, pool);
	
	if(in_arglist) {
		rVal->func->arg_count = in_arglist->depth;

		rVal->func->arg_vector = XaviArglistGetTrees(in_arglist);
		XaviCleanupReleaseArglist(in_arglist, pool);
		XaviArglistDissolve(in_arglist);		
	}
	else {
		rVal->func->arg_count = 0;
		rVal->func->arg_vector = NULL;
	}
	
	return rVal;
}

void XaviTreeDelete(XaviTree * tree)
{
	int i;
	if (tree) {
		switch (tree->type) {
			case 'n':
				free(tree->num);
				break;
			case 'o':
				XaviTreeDelete(tree->op->left);
				XaviTreeDelete(tree->op->right);
				free(tree->op->args);
				free(tree->op);
				break;
			case 'f':
				XaviFunctionIdDelete(tree->func->name);
				for (i = 0; i <  tree->func->arg_count; i++) {
					XaviTreeDelete(tree->func->arg_vector[i]);
				}
				free(tree->func->arg_vector);
				free(tree->func);
				break;
		}
		free(tree);
	}
}
