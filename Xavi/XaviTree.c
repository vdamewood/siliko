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
	if (parent->type == XAVI_NODE_VECTOR_BRANCH)
	{
		if (parent->branch->count == 0)
		{
			return 0;
		}
		else if (parent->branch->vector[0] == NULL)
		{
			parent->branch->vector[0] = left;
			return -1;
		}
		else
		{
			return XaviTreeGraftLeft
			(
				parent->branch->vector[0],
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

XaviTreeNode *XaviTreeNewListBranch(XaviTreeNode * newChild)
{
	XaviTreeNode *rVal;
	XaviTreeBranch *rValBranch;
	XaviTreeListNode *rValListNode;

	if (!(rVal = malloc(sizeof(XaviTreeNode))))
		return NULL;

	if (!(rValBranch = malloc(sizeof(XaviTreeBranch))))
	{
		free(rVal);
		return NULL;
	}

	if (!(rValListNode = malloc(sizeof(XaviTreeListNode))))
	{
		free(rVal->branch);
		free(rVal);
		return NULL;
	}

	rVal->type = XAVI_NODE_LIST_BRANCH;
	rVal->branch = rValBranch;
	rVal->branch->id = NULL;
	rVal->branch->count = 1;
	rVal->branch->list = rValListNode;
	rVal->branch->list->value = newChild;
	rVal->branch->list->next = NULL;

	return rVal;
}

XaviTreeNode *XaviTreeNewVectorBranch(char *id, int count, XaviTreeNode **children)
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

	rVal->type = XAVI_NODE_VECTOR_BRANCH;
	rVal->branch->id = id;
	rVal->branch->count = count;
	rVal->branch->vector = children;

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

void XaviTreeDeleteListNode(XaviTreeListNode *node)
{
	if (node)
	{
		XaviTreeDelete(node->value);
		// FIXME: Valgrind detects the following as an
		// invalid read.
		if (node->next)
			XaviTreeDeleteListNode(node->next);
	}
	free(node);
}

void XaviTreeDelete(XaviTreeNode *node)
{
	int i;
	if (node)
		switch (node->type)
		{
		case XAVI_NODE_VECTOR_BRANCH:
			free(node->branch->id);
			for (i = 0; i <  node->branch->count; i++)
				XaviTreeDelete(node->branch->vector[i]);
			free(node->branch->vector);
			free(node->branch);
			break;
		case XAVI_NODE_LIST_BRANCH:
			free(node->branch->id);
			XaviTreeDeleteListNode(node->branch->list);
			free(node->branch);
			break;
		}

	free(node);
}

static int IsNumber(XaviValue n)
{
	return n.status == XS_INTEGER || n.status == XS_FLOAT;
}

static XaviValue EvaluateVectorBranch(XaviTreeBranch *branch)
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
			arguments[i] = XaviTreeEvaluate(branch->vector[i]);
			if(!IsNumber(arguments[i]))
			{
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}

	rVal = XaviFunctionCall(branch->id, branch->count, arguments);
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
	case XAVI_NODE_VECTOR_BRANCH:
		return EvaluateVectorBranch(node->branch);
	}

	rVal.status = XE_INTERNAL;
	return rVal;
}

int XaviTreeCollapseBranch(XaviTreeNode *collapseNode)
{
	XaviTreeNode **newVector;
	XaviTreeListNode *current;
	XaviTreeListNode *next;
	int i;

	if ((newVector = malloc(collapseNode->branch->count * sizeof(XaviTreeNode *))))
	{
		current = collapseNode->branch->list;
		for (i = 0; i < collapseNode->branch->count; i++)
		{
			newVector[i] = current->value;
			next = current->next;
			free(current);
			current = next;
		}
		collapseNode->type = XAVI_NODE_VECTOR_BRANCH;
		collapseNode->branch->vector = newVector;
		return 1;
	}
	else
	{
		return 0;
	}
}

int XaviTreePushFront(XaviTreeNode *mainBranch, XaviTreeNode *newNode)
{
	XaviTreeListNode *newListNode;

	if (!(newListNode = malloc(sizeof(XaviTreeListNode))))
		return 0;

	newListNode->value = newNode;
	mainBranch->branch->count++;

	newListNode->next = mainBranch->branch->list;
	mainBranch->branch->list = newListNode;
}

int XaviTreePush(XaviTreeNode *mainNode, XaviTreeNode *newNode)
{
	XaviTreeListNode *newListNode;
	XaviTreeListNode *currentListNode;

	if (!(newListNode = malloc(sizeof(XaviTreeListNode))))
		return 0;

	newListNode->value = newNode;
	mainNode->branch->count++;

	currentListNode = mainNode->branch->list;
	while (currentListNode->next)
		currentListNode = currentListNode->next;

	newListNode->next = NULL;
	currentListNode->next = newListNode;
}


/*
XaviArglist *XaviArglistNew(XaviTreeNode *NewTree, XaviArglist *OldList)
{
	XaviArglist *rVal;

	if ((rVal = malloc(sizeof(XaviArglist))))
	{
		if (OldList)
			rVal->depth = OldList->depth + 1;
		else
			rVal->depth = 1;
		rVal->value = NewTree;
		rVal->next = OldList;
	}
	return rVal;
}

void XaviArglistDelete(XaviArglist *OldArglist)
{
	if (OldArglist)
	{
		XaviArglistDelete(OldArglist->next);
		XaviTreeDelete(OldArglist->value);
		free(OldArglist);
	}
}

XaviTreeNode **XaviArglistGetTrees(XaviArglist *InArglist)
{
	XaviTreeNode **rVal;
	XaviArglist *Current = InArglist;
	XaviArglist *next;
	int i;
	int depth = InArglist->depth;

	if ((rVal = malloc(depth * sizeof(XaviTreeNode *))))
	{
		for (i = 0; i < depth; i++)
		{
			rVal[i] = Current->value;
			next = Current->next;
			free(Current);
			Current = next;
		}
	}
	else
	{
		rVal = NULL;
	}

	return rVal;
}
*/
