/*
 * XaviTree.c: Functions to manipulate abstract syntax trees
 * Copyright 2012, 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "SyntaxTree.h"
#include "FunctionCaller.h"

XaviSyntaxTreeNode *XaviSyntaxTreeNewError(void)
{
	XaviSyntaxTreeNode *rVal;
	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->type = XAVI_NODE_ERROR;
		rVal->i = 0;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewNothing(void)
{
	XaviSyntaxTreeNode *rVal;
	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->type = XAVI_NODE_NOTHING;
		rVal->i = 0;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewInteger(int value)
{
	XaviSyntaxTreeNode *rVal;

	if (!(rVal = malloc(sizeof(XaviSyntaxTreeNode))))
		return NULL;

	rVal->type = XAVI_NODE_INTEGER;
	rVal->i = value;
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewFloat(float value)
{
	XaviSyntaxTreeNode *rVal;

	if (!(rVal = malloc(sizeof(XaviSyntaxTreeNode))))
		return NULL;

	rVal->type = XAVI_NODE_FLOAT;
	rVal->f = value;
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewBranch(char *NewId)
{
	const int DefaultSize = 4;
	XaviSyntaxTreeNode *rVal = NULL;
	XaviSyntaxTreeBranch *rValBranch = NULL;
	XaviSyntaxTreeNode **rValChildren = NULL;

	if (!(rVal = malloc(sizeof(XaviSyntaxTreeNode))))
		goto memerr;
	if (!(rValBranch = malloc(sizeof(XaviSyntaxTreeBranch))))
		goto memerr;
	if (!(rValChildren = calloc(DefaultSize, sizeof(XaviSyntaxTreeNode*))))
		goto memerr;

	rVal->type = XAVI_NODE_BRANCH;
	rVal->branch = rValBranch;
	rVal->branch->id = strdup(NewId);
	rVal->branch->count = 0;
	rVal->branch->capacity = DefaultSize;
	rVal->branch->children = rValChildren;

	return rVal;
memerr:
	free(rVal);
	free(rValBranch);
	free(rValChildren);
	return NULL;
}

int XaviSyntaxTreePushRight(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewChild)
{
	const int INCREMENT = 2;
	if (Tree->branch->count == Tree->branch->capacity)
	{
		XaviSyntaxTreeNode **newChildren;
		int newCapacity = Tree->branch->capacity + INCREMENT;

		if (!(newChildren = calloc(newCapacity, sizeof(XaviSyntaxTreeNode*))))
			return 0; // FIXME
		memcpy(newChildren, Tree->branch->children, Tree->branch->capacity * sizeof(XaviSyntaxTreeNode*));
		free(Tree->branch->children);
		Tree->branch->children = newChildren;
		Tree->branch->capacity = newCapacity;
	}

	Tree->branch->children[Tree->branch->count] = NewChild;
	Tree->branch->count++;
	return -1;
}

int XaviSyntaxTreeGraftLeft(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewBranch)
{
	if (Tree->type == XAVI_NODE_BRANCH)
	{
		if (Tree->branch->count == 0)
		{
			return 0;
		}
		else if (Tree->branch->children[0] == NULL)
		{
			Tree->branch->children[0] = NewBranch;
			return -1;
		}
		else
		{
			return XaviSyntaxTreeGraftLeft
				(Tree->branch->children[0], NewBranch);
		}
	}
	else
	{
		return 0;
	}
}

int XaviSyntaxTreeNegate(XaviSyntaxTreeNode *tree)
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

void XaviSyntaxTreeDelete(XaviSyntaxTreeNode *node)
{
	if (node)
		if (node->type == XAVI_NODE_BRANCH)
		{
			free(node->branch->id);
			for (int i = 0; i <  node->branch->count; i++)
				XaviSyntaxTreeDelete(node->branch->children[i]);
			free(node->branch->children);
			free(node->branch);
		}
	free(node);
}

static int IsNumber(XaviValue n)
{
	return n.Status == XAVI_INTEGER || n.Status == XAVI_FLOAT;
}

static XaviValue EvaluateBranch(XaviSyntaxTreeBranch *branch)
{
	XaviValue rVal;
	XaviValue *arguments = NULL;

	if (branch->count)
	{
		if (!(arguments =
			calloc(branch->count, sizeof(XaviValue))))
		{
			rVal.Status = XAVI_MEMORY_ERR;
			return rVal;
		}

		for(int i = 0; i < branch->count; i++)
		{
			arguments[i] = XaviSyntaxTreeEvaluate(branch->children[i]);
			if(!IsNumber(arguments[i]))
			{
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}

	rVal = XaviFunctionCallerCall(branch->id, branch->count, arguments);
	free(arguments);
	return rVal;
}

XaviValue XaviSyntaxTreeEvaluate(XaviSyntaxTreeNode *node)
{
	XaviValue rVal;

	if (!node)
	{
		rVal.Status = XAVI_SYNTAX_ERR;
		return rVal;
	}

	switch (node->type)
	{
	case XAVI_NODE_INTEGER:
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = node->i;
		return rVal;
	case XAVI_NODE_FLOAT:
		rVal.Status = XAVI_FLOAT;
		rVal.Float = node->f;
		return rVal;
	case XAVI_NODE_BRANCH:
		return EvaluateBranch(node->branch);
	default:
		rVal.Status = XAVI_SYNTAX_ERR;
		return rVal;
	}
}

