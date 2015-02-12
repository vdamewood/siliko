/*
 * SyntaxTree.h: Functions to manipulate abstract syntax trees
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

#if !defined XAVI_SYNTAX_TREE_H
#define XAVI_SYNTAX_TREE_H

#include "W32Dll.h"
#include "Value.h"

enum XAVI_API XaviSyntaxTreeNodeType
{
	XAVI_NODE_ERROR = -1,
	XAVI_NODE_NOTHING = 0,
	XAVI_NODE_INTEGER,
	XAVI_NODE_FLOAT,
	XAVI_NODE_BRANCH
};
typedef enum XaviSyntaxTreeNodeType XaviSyntaxTreeNodeType;

struct XaviSyntaxTreeNode;
typedef struct XaviSyntaxTreeNode XaviSyntaxTreeNode;

struct XAVI_API XaviSyntaxTreeBranch
{
	char *Id;
	int Count;
	int Capacity;
	int IsNegated;
	XaviSyntaxTreeNode **Children;
};
typedef struct XaviSyntaxTreeBranch XaviSyntaxTreeBranch;

struct XAVI_API XaviSyntaxTreeNode
{
	XaviSyntaxTreeNodeType Type;
	union
	{
		int Integer;
		float Float;
		XaviSyntaxTreeBranch *Branch;
	};
};

XAVI_API XaviSyntaxTreeNode *XaviSyntaxTreeNewError(void);
XAVI_API XaviSyntaxTreeNode *XaviSyntaxTreeNewNothing(void);
XAVI_API XaviSyntaxTreeNode *XaviSyntaxTreeNewInteger(int Value);
XAVI_API XaviSyntaxTreeNode *XaviSyntaxTreeNewFloat(float Value);
XAVI_API XaviSyntaxTreeNode *XaviSyntaxTreeNewBranch(char *Id);
XAVI_API void XaviSyntaxTreeDelete(XaviSyntaxTreeNode *);

XAVI_API XaviValue XaviSyntaxTreeEvaluate(XaviSyntaxTreeNode *);
XAVI_API int XaviSyntaxTreeNegate(XaviSyntaxTreeNode *tree);
XAVI_API int XaviSyntaxTreePushLeft(XaviSyntaxTreeNode *, XaviSyntaxTreeNode *);
XAVI_API int XaviSyntaxTreePushRight(XaviSyntaxTreeNode *, XaviSyntaxTreeNode *);
XAVI_API int XaviSyntaxTreeGraftLeft(XaviSyntaxTreeNode *, XaviSyntaxTreeNode *);
XAVI_API int XaviSyntaxTreeGraftRight(XaviSyntaxTreeNode *, XaviSyntaxTreeNode *);

#endif // XAVI_SYNTAX_TREE_H
