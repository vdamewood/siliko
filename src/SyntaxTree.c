/* SyntaxTree.c: Functions to manipulate abstract syntax trees
 * Copyright 2012-2025 Vincent Damewood
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

#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/FunctionCaller.h>

#if defined _WIN32
#define strdup _strdup
#endif

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
		SilikoValue *Leaf;
		SilikoSyntaxTreeBranch *Branch;
	};
};

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewError(void)
{
	SilikoSyntaxTreeNode *object = malloc(sizeof(SilikoSyntaxTreeNode));
	if (!object)
		return NULL;

	object->Type = SILIKO_AST_LEAF;
	object->Leaf = SilikoValueNewError(SilikoErrorSyntax);
	if(!object->Leaf)
	{
		free (object);
		return NULL;
	}

	return object;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewNothing(void)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoSyntaxTreeNode))))
		rVal->Type = SILIKO_AST_NOTHING;

	return rVal;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewLeaf(SilikoValue *source)
{
	SilikoSyntaxTreeNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SILIKO_AST_LEAF;
	object->Leaf = SilikoValueNewCopy(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFromInteger(long long int source)
{
	SilikoSyntaxTreeNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SILIKO_AST_LEAF;
	object->Leaf = SilikoValueNewInteger(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewFromFloat(double source)
{
	SilikoSyntaxTreeNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SILIKO_AST_LEAF;
	object->Leaf = SilikoValueNewReal(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoSyntaxTreeNode *SilikoSyntaxTreeNewBranch(const char *NewId)
{
	const int DefaultSize = 4;
	SilikoSyntaxTreeNode *rVal = NULL;
	SilikoSyntaxTreeBranch *rValBranch = NULL;
	SilikoSyntaxTreeNode **rValChildren = NULL;
	char * rValId = NULL;
	if (!(rVal = malloc(sizeof(SilikoSyntaxTreeNode)))
		|| !(rValBranch = malloc(sizeof(SilikoSyntaxTreeBranch)))
		|| !(rValChildren = calloc(DefaultSize,
								sizeof(SilikoSyntaxTreeNode*)))
		|| !(rValId = strdup(NewId)))
	{
		free(rVal);
		free(rValBranch);
		free(rValChildren);
		free(rValId);
		return NULL;
	}

	rVal->Type = SILIKO_AST_BRANCH;
	rVal->Branch = rValBranch;
	rVal->Branch->Id = rValId;
	rVal->Branch->Count = 0;
	rVal->Branch->IsNegated = 0;
	rVal->Branch->Capacity = DefaultSize;
	rVal->Branch->Children = rValChildren;

	return rVal;
}

static int ExpandChildren(SilikoSyntaxTreeNode *Tree)
{
	const int Increment = 2;
	SilikoSyntaxTreeNode **NewChildren = NULL;
	int NewCapacity = Tree->Branch->Capacity + Increment;

	if (!(NewChildren = calloc(NewCapacity, sizeof(SilikoSyntaxTreeNode*))))
		return 0;

	memcpy(NewChildren, Tree->Branch->Children, Tree->Branch->Capacity * sizeof(SilikoSyntaxTreeNode*));
	free(Tree->Branch->Children);

	Tree->Branch->Children = NewChildren;
	Tree->Branch->Capacity = NewCapacity;
	return -1;
}

int SilikoSyntaxTreePushLeft(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	for (int i = Tree->Branch->Count; i >= 1; i--)
		Tree->Branch->Children[i] = Tree->Branch->Children[i-1];

	Tree->Branch->Children[0] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int SilikoSyntaxTreePushRight(SilikoSyntaxTreeNode *Tree, SilikoSyntaxTreeNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	Tree->Branch->Children[Tree->Branch->Count] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

SilikoSyntaxTreeNodeType SilikoSyntaxTreeGetType(SilikoSyntaxTreeNode *SyntaxTree)
{
	return SyntaxTree->Type;
}

int SilikoSyntaxTreeIsError(SilikoSyntaxTreeNode *SyntaxTree)
{
	return
		SyntaxTree->Type == SILIKO_AST_LEAF
		&& SilikoValueGetStatus(SyntaxTree->Leaf) == SilikoValueError;
}

int SilikoSyntaxTreeNegate(SilikoSyntaxTreeNode *Tree)
{
	if (Tree == NULL)
		return 0;

	switch (Tree->Type)
	{
	case SILIKO_AST_LEAF:
		SilikoValueNegate(Tree->Leaf);
		return -1;
	case SILIKO_AST_BRANCH:
		Tree->Branch->IsNegated = !Tree->Branch->IsNegated;
		return -1;
	default:
		return 0;
	}
}

static void DeleteBranch(SilikoSyntaxTreeBranch *branch)
{
	for (int i = 0; i < branch->Count; i++)
		SilikoSyntaxTreeDelete(branch->Children[i]);
	free(branch->Id);
	free(branch->Children);
	free(branch);
}

void SilikoSyntaxTreeDelete(SilikoSyntaxTreeNode *Node)
{
	if (Node && Node->Type == SILIKO_AST_BRANCH)
		DeleteBranch(Node->Branch);
	free(Node);
}

static SilikoValue *EvaluateBranch(SilikoSyntaxTreeBranch *Branch, SilikoFunctionCaller *Caller)
{
	SilikoValue **Arguments = NULL;
	if (Branch->Count)
	{
		if (!(Arguments = calloc(Branch->Count, sizeof(*Arguments))))
			return NULL;

		for(int i = 0; i < Branch->Count; i++)
		{
			Arguments[i] = SilikoSyntaxTreeEvaluate(Branch->Children[i], Caller);
			if(SilikoValueGetStatus(Arguments[i]) == SilikoValueError)
			{
				SilikoValue *error = Arguments[i];
				Arguments[i] = NULL;
				for (int j = 0; j < i; j++)
					SilikoValueDelete(Arguments[j]);
				free(Arguments);
				return error;
			}
		}
	}

	SilikoValue *result
		= SilikoFunctionCallerCall(
			Caller, Branch->Id, Branch->Count, Arguments);
	free(Arguments);

	if (Branch->IsNegated)
		SilikoValueNegate(result);
	return result;
}

SilikoValue *SilikoSyntaxTreeEvaluate(SilikoSyntaxTreeNode *Node, SilikoFunctionCaller *Caller)
{
	if (!Node)
		return SilikoValueNewError(SilikoErrorSyntax);

	switch (Node->Type)
	{
	case SILIKO_AST_LEAF:
		return SilikoValueNewCopy(Node->Leaf);
	case SILIKO_AST_BRANCH:
		return EvaluateBranch(Node->Branch, Caller);
	default: // Shouldn't happen, but just in case.
		return SilikoValueNewError(SilikoErrorSyntax);
	}
}

int SilikoSyntaxTreeCollapse(SilikoSyntaxTreeNode *node, SilikoFunctionCaller *caller)
{
	if (node->Type != SILIKO_AST_BRANCH)
		return 0;

	SilikoValue *new_value = SilikoSyntaxTreeEvaluate(node, caller);
	DeleteBranch(node->Branch);
	node->Type = SILIKO_AST_LEAF;
	node->Leaf = new_value;
	return -1;
}

static inline int checkBounds(SilikoSyntaxTreeNode *parent, int child_index)
{
	if (parent->Type != SILIKO_AST_BRANCH
			|| child_index >= parent->Branch->Count
			|| child_index < -parent->Branch->Count)
		return -1;

	if (child_index < 0)
		return child_index + parent->Branch->Count;

	return child_index;

}

SilikoSyntaxTreeNode *SilikoSyntaxTreeFetchChild(SilikoSyntaxTreeNode *parent, int child_index)
{
	if ((child_index = checkBounds(parent, child_index)) < 0)
		return NULL;

	return parent->Branch->Children[child_index];
}

SilikoSyntaxTreeNode *SilikoSyntaxTreePruneChild(SilikoSyntaxTreeNode *parent, int child_index)
{
	if ((child_index = checkBounds(parent, child_index)) < 0)
		return NULL;

	SilikoSyntaxTreeNode *child = parent->Branch->Children[child_index];
	if (!child)
		return NULL;

	parent->Branch->Count--;
	for (int i = child_index; i < parent->Branch->Count; i++)
		parent->Branch->Children[i] = parent->Branch->Children[i+1];

	parent->Branch->Children[parent->Branch->Count] = NULL;

	return child;
}
