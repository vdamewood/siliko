/*
 * XaviTree.h: Functions to manipulate abstract syntax trees
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

#if !defined Xavi_TREE_H
#define Xavi_TREE_H

#include "XaviValue.h"

enum XaviTreeNodeType
{
	XAVI_NODE_ERROR = -1,
	XAVI_NODE_NOTHING = 0,
	XAVI_NODE_INTEGER,
	XAVI_NODE_FLOAT,
	XAVI_NODE_VECTOR_BRANCH,
	XAVI_NODE_LIST_BRANCH
};
typedef enum XaviTreeNodeType XaviTreeNodeType;

struct XaviTreeNode;
typedef struct XaviTreeNode XaviTreeNode;

struct XaviTreeListNode;
typedef struct XaviTreeListNode XaviTreeListNode;

struct XaviTreeListNode
{
	XaviTreeNode *value;
	XaviTreeListNode *next;
};

struct XaviTreeBranch
{
	char *id;
	int count;
	union
	{
		XaviTreeListNode *list;
		XaviTreeNode **vector;
	};
};
typedef struct XaviTreeBranch XaviTreeBranch;

struct XaviTreeNode
{
	XaviTreeNodeType type;
	union
	{
		int i;
		float f;
		XaviTreeBranch *branch;
	};
};

XaviTreeNode *XaviTreeNewError(void);
XaviTreeNode *XaviTreeNewFloat(float Value);
XaviTreeNode *XaviTreeNewInteger(int Value);
XaviTreeNode *XaviTreeNewListBranch(XaviTreeNode *NewChild);
XaviTreeNode *XaviTreeNewNothing(void);
XaviTreeNode *XaviTreeNewVectorBranch(char *id, int count, XaviTreeNode **Children);
void XaviTreeDelete(XaviTreeNode *TreeToDelete);

XaviValue XaviTreeEvaluate(XaviTreeNode *TreeToEvaluate);
int XaviTreeGraftLeft(XaviTreeNode *parent, XaviTreeNode *left);
int XaviTreeNegate(XaviTreeNode *TreeToNegate);
int XaviTreeCollapseBranch(XaviTreeNode *TreeToCollapse);
int XaviTreePushFront(XaviTreeNode *MainBranch, XaviTreeNode *NewNode);
int XaviTreePush(XaviTreeNode *MainBranch, XaviTreeNode *NewNode);
#endif // Xavi_TREE_H
