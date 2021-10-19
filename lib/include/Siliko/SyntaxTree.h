/* SyntaxTree.h: Functions to manipulate abstract syntax trees
 * Copyright 2012-2021 Vincent Damewood
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

#include <Siliko/Api.h>
#include <Siliko/Value.h>

#if defined __cplusplus
extern "C" {
#endif

enum SilikoSyntaxTreeNodeType
{
	SILIKO_AST_NOTHING = 0,
	SILIKO_AST_BRANCH,
	SILIKO_AST_LEAF
};
typedef enum SilikoSyntaxTreeNodeType SilikoSyntaxTreeNodeType;

struct SilikoSyntaxTreeNode;
typedef struct SilikoSyntaxTreeNode SilikoSyntaxTreeNode;

struct SilikoSyntaxTreeBranch
{
	char *Id;
	int Count;
	int Capacity;
	int IsNegated;
	SilikoSyntaxTreeNode **Children;
};
typedef struct SilikoSyntaxTreeBranch SilikoSyntaxTreeBranch;

struct SilikoSyntaxTreeNode
{
	SilikoSyntaxTreeNodeType Type;
	union
	{
		SilikoValue Leaf;
		SilikoSyntaxTreeBranch *Branch;
	};
};

SILIKO_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewInteger(long long int IntegerValue);
SILIKO_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFloat(double FloatValue);
SILIKO_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewBranch(char *Id);
SILIKO_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewError(void);
SILIKO_EXPORT SilikoSyntaxTreeNode *SilikoSyntaxTreeNewNothing(void);
SILIKO_EXPORT void SilikoSyntaxTreeDelete(SilikoSyntaxTreeNode *SyntaxTree);

SILIKO_EXPORT SilikoValue SilikoSyntaxTreeEvaluate(SilikoSyntaxTreeNode *SyntaxTree);
SILIKO_EXPORT int SilikoSyntaxTreeNegate(SilikoSyntaxTreeNode *SyntaxTree);

SILIKO_EXPORT int SilikoSyntaxTreePushLeft(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_EXPORT int SilikoSyntaxTreePushRight(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_EXPORT int SilikoSyntaxTreeGraftLeft(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);
SILIKO_EXPORT int SilikoSyntaxTreeGraftRight(SilikoSyntaxTreeNode *BaseTree, SilikoSyntaxTreeNode *NewBranch);

#if defined __cplusplus
}
#endif

#endif // SILIKO_SYNTAX_TREE_H
