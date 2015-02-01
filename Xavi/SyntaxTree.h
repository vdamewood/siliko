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

#include "W32Dll.h"
#include "Value.h"

enum XAVI_API XaviTreeNodeType
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

struct XAVI_API XaviTreeListNode
{
	XaviTreeNode *value;
	XaviTreeListNode *next;
};

struct XAVI_API XaviTreeBranch
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

struct XAVI_API XaviTreeNode
{
	XaviTreeNodeType type;
	union
	{
		int i;
		float f;
		XaviTreeBranch *branch;
	};
};

XAVI_API XaviTreeNode *XaviTreeNewError(void);
XAVI_API XaviTreeNode *XaviTreeNewFloat(float Value);
XAVI_API XaviTreeNode *XaviTreeNewInteger(int Value);
XAVI_API XaviTreeNode *XaviTreeNewListBranch(XaviTreeNode *NewChild);
XAVI_API XaviTreeNode *XaviTreeNewNothing(void);
XAVI_API XaviTreeNode *XaviTreeNewVectorBranch(char *id, int count, XaviTreeNode **Children);
XAVI_API void XaviTreeDelete(XaviTreeNode *TreeToDelete);

XAVI_API XaviValue XaviTreeEvaluate(XaviTreeNode *TreeToEvaluate);
XAVI_API int XaviTreeGraftLeft(XaviTreeNode *parent, XaviTreeNode *left);
XAVI_API int XaviTreeNegate(XaviTreeNode *TreeToNegate);
XAVI_API int XaviTreeCollapseBranch(XaviTreeNode *TreeToCollapse);
XAVI_API int XaviTreePushFront(XaviTreeNode *MainBranch, XaviTreeNode *NewNode);
XAVI_API int XaviTreePush(XaviTreeNode *MainBranch, XaviTreeNode *NewNode);
#endif // Xavi_TREE_H
