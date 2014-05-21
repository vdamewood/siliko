/*
 * XaviTree.h: Functions to manipulate abstract syntax trees.
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

#if !defined Xavi_TREE_H
#define Xavi_TREE_H

#include "XaviValue.h"

enum XaviTreeNodeType
{
	XAVI_NODE_ERROR = 0,
	XAVI_NODE_INTEGER,
	XAVI_NODE_FLOAT,
	XAVI_NODE_BRANCH
};
typedef enum XaviTreeNodeType XaviTreeNodeType;

struct XaviTreeNode;
typedef struct XaviTreeNode XaviTreeNode;

struct XaviTreeBranch
{
	char *name;
	int count;
	XaviTreeNode **children;
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

XaviValue XaviTreeEvaluate(XaviTreeNode *TreeToEvaluate);
int XaviTreeGraftLeft(XaviTreeNode *parent, XaviTreeNode *left);
int XaviTreeNegate(XaviTreeNode *TreeToNegate);
XaviTreeNode *XaviTreeNewError(void);
XaviTreeNode *XaviTreeNewInteger(int Value);
XaviTreeNode *XaviTreeNewFloat(float Value);
XaviTreeNode *XaviTreeNewBranch(char *id, int count, XaviTreeNode **children);
void XaviTreeDelete(XaviTreeNode *TreeToDelete);

#endif // Xavi_TREE_H
