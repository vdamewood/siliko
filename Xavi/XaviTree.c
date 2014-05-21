/*
 * XaviTree.c: Functions to manipulate abstract syntax trees.
 * Copyright 2012, 2014 Vincent Damewood
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
#include "XaviFunctionCall.h"

int XaviTreeGraftLeft(XaviTreeNode *parent, XaviTreeNode *left)
{
	if (parent->type == XAVI_NODE_BRANCH)
	{
		if (parent->branch->count == 0)
		{
			return 0;
		}
		else if (parent->branch->children[0] == NULL)
		{
			parent->branch->children[0] = left;
			return -1;
		}
		else
		{
			return XaviTreeGraftLeft
			(
				parent->branch->children[0],
				left
			);
		}
	}
	else
	{
		return 0;
	}
}

int XaviTreeNegate(XaviTreeNode *tree)
{
	if (tree == NULL)
		return 0;

	switch (tree->type)
	{
	case XAVI_NODE_INTEGER:
		tree->i *= -1;
		return -1;
	case XAVI_NODE_FLOAT:
		tree->f *= -1.0;
		return -1;
	default:
		return 0;
	}
}

XaviTreeNode *XaviTreeNewNothing(void)
{
	XaviTreeNode *rVal;
	if ((rVal = malloc(sizeof(XaviTreeNode))))
	{
		rVal->type = XAVI_NODE_NOTHING;
		rVal->i = 0;
	}
	return rVal;
}

XaviTreeNode *XaviTreeNewInteger(int value)
{
	XaviTreeNode *rVal;

	if (!(rVal = malloc(sizeof(XaviTreeNode))))
		return NULL;

	rVal->type = XAVI_NODE_INTEGER;
	rVal->i = value;
	return rVal;
}

XaviTreeNode *XaviTreeNewFloat(float value)
{
	XaviTreeNode *rVal;

	if (!(rVal = malloc(sizeof(XaviTreeNode))))
		return NULL;

	rVal->type = XAVI_NODE_FLOAT;
	rVal->f = value;
	return rVal;
}

XaviTreeNode *XaviTreeNewBranch(char *name, int count, XaviTreeNode **children)
{
	XaviTreeNode *rVal;
	XaviTreeBranch *rValBranch;

	rVal = malloc(sizeof(XaviTreeNode));
	rValBranch = malloc(sizeof(XaviTreeBranch));

	if (!rVal || !rValBranch)
	{
		free(rVal);
		free(rValBranch);
		return NULL;
	}
	rVal->branch = rValBranch;

	rVal->type = XAVI_NODE_BRANCH;
	rVal->branch->name = name;
	rVal->branch->count = count;
	rVal->branch->children = children;

	return rVal;
}

XaviTreeNode *XaviTreeNewError(void)
{
	XaviTreeNode *rVal;
	if ((rVal = malloc(sizeof(XaviTreeNode))))
	{
		rVal->type = XAVI_NODE_ERROR;
		rVal->i = 0;
	}
	return rVal;
}

void XaviTreeDelete(XaviTreeNode *node)
{
	int i;
	if (node && node->type == XAVI_NODE_BRANCH)
	{
		free(node->branch->name);
		for (i = 0; i <  node->branch->count; i++)
			XaviTreeDelete(node->branch->children[i]);
		free(node->branch->children);
		free(node->branch);
	}
	free(node);
}

static int IsNumber(XaviValue n)
{
	return n.status == XS_INTEGER || n.status == XS_FLOAT;
}

static XaviValue EvaluateBranch(XaviTreeBranch *branch)
{
	XaviValue rVal;
	XaviValue *arguments = NULL;
	int i;

	if (branch->count)
	{
		if (!(arguments =
			malloc(sizeof(XaviValue) * branch->count)))
		{
			rVal.status = XE_MEMORY;
			return rVal;
		}

		for(i = 0; i < branch->count; i++)
		{
			arguments[i] = XaviTreeEvaluate(branch->children[i]);
			if(!IsNumber(arguments[i]))
			{
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}

	rVal = XaviFunctionCall(branch->name, branch->count, arguments);
	free(arguments);
	return rVal;
}

XaviValue XaviTreeEvaluate(XaviTreeNode *node)
{
	XaviValue rVal;

	if (!node || node->type == XAVI_NODE_ERROR)
	{
		rVal.status = XE_SYNTAX;
		return rVal;
	}

	switch (node->type)
	{
	case XAVI_NODE_INTEGER:
		rVal.status = XS_INTEGER;
		rVal.i = node->i;
		return rVal;
	case XAVI_NODE_FLOAT:
		rVal.status = XS_FLOAT;
		rVal.f = node->f;
		return rVal;
	case XAVI_NODE_BRANCH:
		return EvaluateBranch(node->branch);
	}

	rVal.status = XE_INTERNAL;
	return rVal;
}
