/*
 * SyntaxTree.c: Functions to manipulate abstract syntax trees
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

#if defined _WIN32
#define strdup _strdup
#endif

XaviSyntaxTreeNode *XaviSyntaxTreeNewError(void)
{
	XaviSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->Type = XAVI_AST_ERROR;
		rVal->Integer = 0;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewNothing(void)
{
	XaviSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->Type = XAVI_AST_NOTHING;
		rVal->Integer = 0;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewInteger(int NewValue)
{
	XaviSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->Type = XAVI_AST_INTEGER;
		rVal->Integer = NewValue;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewFloat(float NewValue)
{
	XaviSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(XaviSyntaxTreeNode))))
	{
		rVal->Type = XAVI_AST_FLOAT;
		rVal->Float = NewValue;
	}
	return rVal;
}

XaviSyntaxTreeNode *XaviSyntaxTreeNewBranch(char *NewId)
{
	const int DefaultSize = 4;
	XaviSyntaxTreeNode *rVal = NULL;
	XaviSyntaxTreeBranch *rValBranch = NULL;
	XaviSyntaxTreeNode **rValChildren = NULL;
	char * rValId = NULL;
	if (!(rVal = malloc(sizeof(XaviSyntaxTreeNode))))
		goto memerr;
	if (!(rValBranch = malloc(sizeof(XaviSyntaxTreeBranch))))
		goto memerr;
	if (!(rValChildren = calloc(DefaultSize, sizeof(XaviSyntaxTreeNode*))))
		goto memerr;
	if (!(rValId = strdup(NewId)))
		goto memerr;

	rVal->Type = XAVI_AST_BRANCH;
	rVal->Branch = rValBranch;
	rVal->Branch->Id = rValId;
	rVal->Branch->Count = 0;
	rVal->Branch->IsNegated = 0;
	rVal->Branch->Capacity = DefaultSize;
	rVal->Branch->Children = rValChildren;

	return rVal;
memerr:
	free(rVal);
	free(rValBranch);
	free(rValChildren);
	free(rValId);
	return NULL;
}

static int ExpandChildren(XaviSyntaxTreeNode *Tree)
{
	const int Increment = 2;
	XaviSyntaxTreeNode **NewChildren = NULL;
	int NewCapacity = Tree->Branch->Capacity + Increment;

	if (!(NewChildren = calloc(NewCapacity, sizeof(XaviSyntaxTreeNode*))))
		return 0;

	memcpy(NewChildren, Tree->Branch->Children, Tree->Branch->Capacity * sizeof(XaviSyntaxTreeNode*));
	free(Tree->Branch->Children);

	Tree->Branch->Children = NewChildren;
	Tree->Branch->Capacity = NewCapacity;
	return -1;
}

int XaviSyntaxTreePushLeft(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	for (int i = Tree->Branch->Count; i < 0; i--)
		Tree->Branch->Children[i] = Tree->Branch->Children[i-1];

	Tree->Branch->Children[0] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int XaviSyntaxTreePushRight(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	Tree->Branch->Children[Tree->Branch->Count] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int XaviSyntaxTreeGraftLeft(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewBranch)
{
	if (Tree->Type == XAVI_AST_BRANCH)
	{
		if (Tree->Branch->Count == 0)
		{
			return 0;
		}
		else if (Tree->Branch->Children[0] == NULL)
		{
			Tree->Branch->Children[0] = NewBranch;
			return -1;
		}
		else
		{
			return XaviSyntaxTreeGraftLeft
				(Tree->Branch->Children[0], NewBranch);
		}
	}
	else
	{
		return 0;
	}
}

int XaviSyntaxTreeGraftRight(XaviSyntaxTreeNode *Tree, XaviSyntaxTreeNode *NewBranch)
{
	if (Tree->Type == XAVI_AST_BRANCH)
	{
		if (Tree->Branch->Count == 0)
		{
			return 0;
		}
		else if (Tree->Branch->Children[Tree->Branch->Count - 1] == NULL)
		{
			Tree->Branch->Children[Tree->Branch->Count - 1] = NewBranch;
			return -1;
		}
		else
		{
			return XaviSyntaxTreeGraftRight(
				Tree->Branch->Children[Tree->Branch->Count - 1],
				NewBranch);
		}
	}
	else
	{
		return 0;
	}
}

int XaviSyntaxTreeNegate(XaviSyntaxTreeNode *Tree)
{
	if (Tree == NULL)
		return 0;

	switch (Tree->Type)
	{
	case XAVI_AST_INTEGER:
		Tree->Integer *= -1;
		return -1;
	case XAVI_AST_FLOAT:
		Tree->Float *= -1.0;
		return -1;
	case XAVI_AST_BRANCH:
		Tree->Branch->IsNegated = !Tree->Branch->IsNegated;
		return -1;
	default:
		return 0;
	}
}

void XaviSyntaxTreeDelete(XaviSyntaxTreeNode *Node)
{
	if (Node)
		if (Node->Type == XAVI_AST_BRANCH)
		{
			free(Node->Branch->Id);
			for (int i = 0; i < Node->Branch->Count; i++)
				XaviSyntaxTreeDelete(Node->Branch->Children[i]);
			free(Node->Branch->Children);
			free(Node->Branch);
		}
	free(Node);
}

static int IsNumber(XaviValue n)
{
	return n.Status == XAVI_VAL_INTEGER || n.Status == XAVI_VAL_FLOAT;
}

static XaviValue EvaluateBranch(XaviSyntaxTreeBranch *Branch)
{
	XaviValue rVal;
	XaviValue *Arguments = NULL;

	if (Branch->Count)
	{
		if (!(Arguments =
			calloc(Branch->Count, sizeof(XaviValue))))
		{
			rVal.Status = XAVI_VAL_MEMORY_ERR;
			return rVal;
		}

		for(int i = 0; i < Branch->Count; i++)
		{
			Arguments[i] = XaviSyntaxTreeEvaluate(Branch->Children[i]);
			if(!IsNumber(Arguments[i]))
			{
				rVal = Arguments[i];
				free(Arguments);
				return rVal;
			}
		}
	}

	rVal = XaviFunctionCallerCall(Branch->Id, Branch->Count, Arguments);
	free(Arguments);

	if (Branch->IsNegated)
	{
		if (rVal.Status == XAVI_VAL_INTEGER)
			rVal.Integer *= -1;
		else if (rVal.Status == XAVI_VAL_FLOAT)
			rVal.Float *= -1.0;
	}

	return rVal;
}

XaviValue XaviSyntaxTreeEvaluate(XaviSyntaxTreeNode *Node)
{
	XaviValue rVal;

	if (!Node)
	{
		rVal.Status = XAVI_VAL_SYNTAX_ERR;
		return rVal;
	}

	switch (Node->Type)
	{
	case XAVI_AST_INTEGER:
		rVal.Status = XAVI_VAL_INTEGER;
		rVal.Integer = Node->Integer;
		return rVal;
	case XAVI_AST_FLOAT:
		rVal.Status = XAVI_VAL_FLOAT;
		rVal.Float = Node->Float;
		return rVal;
	case XAVI_AST_BRANCH:
		return EvaluateBranch(Node->Branch);
	default:
		rVal.Status = XAVI_VAL_SYNTAX_ERR;
		return rVal;
	}
}
