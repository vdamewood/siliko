/* SyntaxTree.h: Functions to manipulate abstract syntax trees
 * Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

#if !defined SILIKO_SYNTAX_TREE_H
#define SILIKO_SYNTAX_TREE_H

#include "W32Dll.h"
#include "Value.h"

#if defined __cplusplus
extern "C" {
#endif

enum SILIKO_API SilikoSyntaxTreeNodeType
{
	SILIKO_AST_ERROR = -1,
	SILIKO_AST_NOTHING = 0,
	SILIKO_AST_INTEGER,
	SILIKO_AST_FLOAT,
	SILIKO_AST_BRANCH
};
typedef enum SilikoSyntaxTreeNodeType SilikoSyntaxTreeNodeType;

struct SilikoSyntaxTreeNode;
typedef struct SilikoSyntaxTreeNode SilikoSyntaxTreeNode;

struct SILIKO_API SilikoSyntaxTreeBranch
{
	char *Id;
	int Count;
	int Capacity;
	int IsNegated;
	SilikoSyntaxTreeNode **Children;
};
typedef struct SilikoSyntaxTreeBranch SilikoSyntaxTreeBranch;

struct SILIKO_API SilikoSyntaxTreeNode
{
	SilikoSyntaxTreeNodeType Type;
	union
	{
		long long int Integer;
		double Float;
		SilikoSyntaxTreeBranch *Branch;
	};
};

SILIKO_API SilikoSyntaxTreeNode *SilikoSyntaxTreeNewInteger(long long int IntegerValue);
SILIKO_API SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFloat(double FloatValue);
SILIKO_API SilikoSyntaxTreeNode *SilikoSyntaxTreeNewBranch(char *Id);
SILIKO_API SilikoSyntaxTreeNode *SilikoSyntaxTreeNewError(void);
SILIKO_API SilikoSyntaxTreeNode *SilikoSyntaxTreeNewNothing(void);
SILIKO_API void SilikoSyntaxTreeDelete(SilikoSyntaxTreeNode *SyntaxTree);

SILIKO_API SilikoValue SilikoSyntaxTreeEvaluate(SilikoSyntaxTreeNode *SyntaxTree);
SILIKO_API int SilikoSyntaxTreeNegate(SilikoSyntaxTreeNode *SyntaxTree);

SILIKO_API int SilikoSyntaxTreePushLeft(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_API int SilikoSyntaxTreePushRight(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_API int SilikoSyntaxTreeGraftLeft(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_API int SilikoSyntaxTreeGraftRight(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);

#if defined __cplusplus
}
#endif

#endif // SILIKO_SYNTAX_TREE_H
