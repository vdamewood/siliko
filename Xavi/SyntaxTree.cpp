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

#include "SyntaxTree.hpp"
#include "FunctionCall.hpp"

Xavi::SyntaxTreeNode::~SyntaxTreeNode()
{
}

Xavi::IntegerNode::IntegerNode(int NewValue)
{
	Value = NewValue;
}

XaviValue Xavi::IntegerNode::GetValue()
{
	XaviValue rVal;
	rVal.status = XS_INTEGER;
	rVal.i = Value;
	return rVal;
}

void Xavi::IntegerNode::Negate()
{
	Value *= -1;
}

Xavi::FloatNode::FloatNode(float NewValue)
{
	Value = NewValue;
}

XaviValue Xavi::FloatNode::GetValue()
{
	XaviValue rVal;
	rVal.status = XS_FLOAT;
	rVal.f = Value;
	return rVal;
}

void Xavi::FloatNode::Negate()
{
	Value *= -1.0;
}

Xavi::BranchNode::BranchNode(void)
{
	Children = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
}

Xavi::BranchNode::BranchNode(std::string NewId)
{
	Children = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
	FunctionId = NewId;
}

void Xavi::BranchNode::SetId(std::string NewId)
{
	FunctionId = NewId;
}

XaviValue Xavi::BranchNode::GetValue()
{
	XaviValue rVal;
	XaviValue *arguments = NULL;

	if (Children.size())
	{
		if (!(arguments =
			(XaviValue*) malloc(sizeof(XaviValue) * Children.size())))
		{
			rVal.status = XE_MEMORY;
			return rVal;
		}

		std::list<Xavi::SyntaxTreeNode *>::iterator i;
		int j;
		for
		(
			i = Children.begin(), j = 0;
			i != Children.end();
			i++, j++
		)
		{
			XaviValue Current = (*i)->GetValue();
			if (Current.status != XS_INTEGER && Current.status != XS_FLOAT)
			{
				free(arguments);
				return Current;
			}
			else
			{
				arguments[j] = Current;
			}
		}
	}

	rVal = XaviFunctionCall(FunctionId.c_str(), Children.size(), arguments);
	free(arguments);
	return rVal;
}

void Xavi::BranchNode::Negate()
{
	IsNegated = !IsNegated;
}

void Xavi::BranchNode::PushLeft(Xavi::SyntaxTreeNode *NewChild)
{
	Children.push_front(NewChild);
}

void Xavi::BranchNode::PushRight(Xavi::SyntaxTreeNode *NewChild)
{
	Children.push_back(NewChild);
}


bool Xavi::BranchNode::GraftLeft(Xavi::SyntaxTreeNode *NewChild)
{
	if (Children.size() == 0)
	{
		return false;
	}
	else if (Children.front() == 0)
	{
		Children.front() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(Children.front()))
			return ChildBranch->GraftLeft(NewChild);
		else
			return false;
	}
}

bool Xavi::BranchNode::GraftRight(Xavi::SyntaxTreeNode *NewChild)
{
	if (Children.size() == 0)
	{
		return false;
	}
	else if (Children.back() == 0)
	{
		Children.back() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(Children.back()))
			return ChildBranch->GraftRight(NewChild);
		else
			return false;
	}
}

Xavi::SyntaxErrorNode::~SyntaxErrorNode(void)
{
}

XaviValue Xavi::SyntaxErrorNode::GetValue(void)
{
	XaviValue rVal;
	rVal.status = XE_SYNTAX;
	rVal.i = 0;
	return rVal;
}

void Xavi::SyntaxErrorNode::Negate(void)
{
}


/*
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
}*/

/*
XaviTreeNode *XaviTreeNewNothing(void)
{
	XaviTreeNode *rVal;
	if ((rVal = (XaviTreeNode *) malloc(sizeof(XaviTreeNode))))
	{
		rVal->type = XAVI_NODE_NOTHING;
		rVal->i = 0;
	}
	return rVal;
}
*/

/*XaviTreeNode *XaviTreeNewInteger(int value)
{
	XaviTreeNode *rVal;

	if (!(rVal = (XaviTreeNode *) malloc(sizeof(XaviTreeNode))))
		return NULL;

	rVal->type = XAVI_NODE_INTEGER;
	rVal->i = value;
	return rVal;
}

XaviTreeNode *XaviTreeNewFloat(float value)
{
	XaviTreeNode *rVal;

	if (!(rVal = (XaviTreeNode*) malloc(sizeof(XaviTreeNode))))
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

	if (!(rVal = (XaviTreeNode*) malloc(sizeof(XaviTreeNode))))
		return NULL;

	if (!(rValBranch = (XaviTreeBranch*) malloc(sizeof(XaviTreeBranch))))
	{
		free(rVal);
		return NULL;
	}

	if (!(rValListNode = (XaviTreeListNode*) malloc(sizeof(XaviTreeListNode))))
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

	rVal = (XaviTreeNode*) malloc(sizeof(XaviTreeNode));
	rValBranch = (XaviTreeBranch*) malloc(sizeof(XaviTreeBranch));

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
	if ((rVal = (XaviTreeNode*) malloc(sizeof(XaviTreeNode))))
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
		default:
			;
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
			(XaviValue*) malloc(sizeof(XaviValue) * branch->count)))
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
	default:
		rVal.status = XE_INTERNAL;
		return rVal;
	}
}

int XaviTreeCollapseBranch(XaviTreeNode *collapseNode)
{
	XaviTreeNode **newVector;
	XaviTreeListNode *current;
	XaviTreeListNode *next;
	int i;

	if ((newVector = (XaviTreeNode**) malloc(collapseNode->branch->count * sizeof(XaviTreeNode *))))
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

	if (!(newListNode = (XaviTreeListNode*) malloc(sizeof(XaviTreeListNode))))
		return 0;

	newListNode->value = newNode;
	mainBranch->branch->count++;

	newListNode->next = mainBranch->branch->list;
	mainBranch->branch->list = newListNode;

	return 1;
}

int XaviTreePush(XaviTreeNode *mainNode, XaviTreeNode *newNode)
{
	XaviTreeListNode *newListNode;
	XaviTreeListNode *currentListNode;

	if (!(newListNode = (XaviTreeListNode*) malloc(sizeof(XaviTreeListNode))))
		return 0;

	newListNode->value = newNode;
	mainNode->branch->count++;

	currentListNode = mainNode->branch->list;
	while (currentListNode->next)
		currentListNode = currentListNode->next;

	newListNode->next = NULL;
	currentListNode->next = newListNode;

	return 1;
}
*/
