// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Siliko.

// Siliko is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Siliko is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Siliko. If not, see
// <http://www.gnu.org/licenses/>.


#include <stdlib.h>
#include <string.h>

#include <SilikoCore/Node.h>
#include <SilikoCore/FunctionCaller.h>

#if defined _WIN32
#define strdup _strdup
#endif

struct Branch
{
	char *Id;
	int Count;
	int Capacity;
	int IsNegated;
	SilikoNode **Children;
};

static void DeleteBranch(struct Branch *branch)
{
	for (int i = 0; i < branch->Count; i++)
		SilikoNodeDelete(branch->Children[i]);
	free(branch->Id);
	free(branch->Children);
	free(branch);
}

struct SilikoNode
{
	enum SilikoNodeStatus Type;
	union
	{
		SilikoValue *Leaf;
		struct Branch *Branch;
	};
};

static inline int checkBounds(const SilikoNode *parent, int child_index)
{
	if (parent->Type != SilikoNodeBranch
			|| child_index >= parent->Branch->Count
			|| child_index < -parent->Branch->Count)
		return -1;

	if (child_index < 0)
		return child_index + parent->Branch->Count;

	return child_index;
}

SilikoNode *SilikoNodeNewNothing(void)
{
	SilikoNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoNode))))
		rVal->Type = SilikoNodeNothing;

	return rVal;
}

SilikoNode *SilikoNodeNewFromError(enum SilikoError source)
{
	SilikoNode *object = malloc(sizeof(SilikoNode));
	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueNewFromError(source);
	if(!object->Leaf)
	{
		free (object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeNewFromInteger(long long int source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueNewFromInteger(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeNewFromReal(double source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueNewFromReal(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeNewFromValue(SilikoValue *source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueNewCopy(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

static struct Branch *NewBranch(const char* source_id)
{
	const int DefaultSize = 4;
	struct Branch *object = malloc(sizeof(*object));
	SilikoNode **new_children = calloc(DefaultSize, sizeof(SilikoNode*));
	char *new_id = strdup(source_id);
	if (!object || !new_children || !new_id)
	{
		free(object);
		free(new_children);
		free(new_id);
		return NULL;
	}

	object->Id = new_id;
	object->Count = 0;
	object->IsNegated = 0;
	object->Capacity = DefaultSize;
	object->Children = new_children;

	return object;
}

SilikoNode *SilikoNodeNewBranch(const char *NewId)
{
	const int DefaultSize = 4;
	SilikoNode *object = malloc(sizeof(SilikoNode));
	if (!object)
		return NULL;

	struct Branch *new_branch = NewBranch(NewId);
	if (!new_branch)
	{
		free(object);
		return NULL;
	}

	object->Type = SilikoNodeBranch;
	object->Branch = new_branch;
	return object;
}

SilikoNode *SilikoNodeNewCopy(const SilikoNode *source)
{
	SilikoNode *object = SilikoNodeNewNothing();
	SilikoNodeCopy(object, source);
	return object;
}

void SilikoNodeAssignFromError(SilikoNode *object, enum SilikoError source)
{
	SilikoValue *new_leaf = SilikoValueNewFromError(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignFromInteger(SilikoNode *object, long long int source)
{
	SilikoValue *new_leaf = SilikoValueNewFromInteger(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignFromReal(SilikoNode *object, double source)
{
	SilikoValue *new_leaf = SilikoValueNewFromReal(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}


void SilikoNodeAssignValue(SilikoNode *object, const SilikoValue *source)
{
	SilikoValue *new_leaf = SilikoValueNewCopy(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignBranch(SilikoNode *object, const char *source)
{
	struct Branch *new_branch = NewBranch(source);
	if (!new_branch)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeBranch;
	object->Branch = new_branch;
}

void SilikoNodeCopy(SilikoNode *object, const SilikoNode *source)
{
	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = source->Type;
	switch (object->Type)
	{
	case SilikoNodeLeaf:
		object->Leaf = SilikoValueNewCopy(source->Leaf);
		if (!object->Leaf)
		{
			free(object);
			object = NULL;
			return;
		}
		break;
	case SilikoNodeBranch:
	{
		object->Branch = malloc(sizeof(*object->Branch));
		if (!object->Branch)
		{
			free(object);
			object = NULL;
			return;
		}
		object->Branch->Id = strdup(source->Branch->Id);
		if (!object->Branch->Id)
		{
			free(object->Branch);
			free(object);
			object = NULL;
			return;
		}
		object->Branch->Count = source->Branch->Count;
		object->Branch->Capacity = source->Branch->Capacity;
		object->Branch->IsNegated = source->Branch->IsNegated;
		object->Branch->Children = calloc(
			object->Branch->Capacity,
			sizeof(*object->Branch->Children));
		if (!object->Branch->Children)
		{
			free(object->Branch->Id);
			free(object->Branch);
			free(object);
			object = NULL;
			return;	
		}
		for (int i = 0; i <= object->Branch->Count; i++)
		{
			object->Branch->Children[i]
				= SilikoNodeNewCopy(source->Branch->Children[i]);
		}
		break;
	}
	default:
		;
	}
}

static int ExpandChildren(SilikoNode *Tree)
{
	const int Increment = 2;
	SilikoNode **NewChildren = NULL;
	int NewCapacity = Tree->Branch->Capacity + Increment;

	if (!(NewChildren = calloc(NewCapacity, sizeof(SilikoNode*))))
		return 0;

	memcpy(NewChildren, Tree->Branch->Children, Tree->Branch->Capacity * sizeof(SilikoNode*));
	free(Tree->Branch->Children);

	Tree->Branch->Children = NewChildren;
	Tree->Branch->Capacity = NewCapacity;
	return -1;
}

int SilikoNodePushCopyLeft(SilikoNode *object, const SilikoNode *new_child)
{
	SilikoNode *new_child_copy = SilikoNodeNewCopy(new_child);
	if (!new_child_copy)
		return 0;
	
	if (!SilikoNodePushLeft(object, new_child_copy))
	{
		SilikoNodeDelete(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodePushLeft(SilikoNode *Tree, SilikoNode *NewChild)
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

int SilikoNodePushCopyRight(SilikoNode *object, const SilikoNode *new_child)
{
	SilikoNode *new_child_copy = SilikoNodeNewCopy(new_child);
	if (!new_child_copy)
		return 0;
	
	if (!SilikoNodePushRight(object, new_child_copy))
	{
		SilikoNodeDelete(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodePushRight(SilikoNode *Tree, SilikoNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;

	Tree->Branch->Children[Tree->Branch->Count] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

int SilikoNodeInsertCopy(SilikoNode *object, int position, const SilikoNode *new_child)
{
	SilikoNode *new_child_copy = SilikoNodeNewCopy(new_child);
	if (!new_child_copy)
		return 0;
	
	if (!SilikoNodeInsert(object, position, new_child_copy))
	{
		SilikoNodeDelete(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodeInsert(SilikoNode *Tree, int position, SilikoNode *NewChild)
{
	if (Tree->Branch->Count == Tree->Branch->Capacity)
		if (!ExpandChildren(Tree))
			return 0;
	
	position = checkBounds(Tree, position);
	if (position < 0)
		return 0;

	for (int i = Tree->Branch->Count; i > position; i--)
		Tree->Branch->Children[i] = Tree->Branch->Children[i-1];

	Tree->Branch->Children[position] = NewChild;
	Tree->Branch->Count++;
	return -1;
}

enum SilikoNodeStatus SilikoNodeGetStatus(const SilikoNode *SyntaxTree)
{
	return SyntaxTree->Type;
}

const SilikoValue *SilikoNodeGetValue(const SilikoNode *object)
{
	return object->Type == SilikoNodeLeaf
		? object->Leaf
		: NULL;
}

const char *SilikoNodeGetId(const SilikoNode *object)
{
	if (object->Type != SilikoNodeBranch)
		return NULL;
	
	return object->Branch->Id;
}

int SilikoNodeIsNegated(const SilikoNode *object)
{
	switch (object->Type)
	{
	case SilikoNodeLeaf:
		switch (SilikoValueGetStatus(object->Leaf))
		{
		case SilikoValueInteger:
			return SilikoValueGetInteger(object->Leaf) < 0;
		case SilikoValueReal:
			return SilikoValueGetReal(object->Leaf) < 0.0;
		default:
			return 0;
		}
	case SilikoNodeBranch:
		return object->Branch->IsNegated;
	default:
		return 0;
	}
}

int SilikoNodeNegate(SilikoNode *Tree)
{
	if (Tree == NULL)
		return 0;

	switch (Tree->Type)
	{
	case SilikoNodeLeaf:
		SilikoValueNegate(Tree->Leaf);
		return -1;
	case SilikoNodeBranch:
		Tree->Branch->IsNegated = !Tree->Branch->IsNegated;
		return -1;
	default:
		return 0;
	}
}

void SilikoNodeDelete(SilikoNode *Node)
{
	if (Node && Node->Type == SilikoNodeBranch)
		DeleteBranch(Node->Branch);
	free(Node);
}

int SilikoNodeCountChildren(const SilikoNode *object)
{
	if (object->Type != SilikoNodeBranch)
		return 0;
	
	return object->Branch->Count;
}

const SilikoNode *SilikoNodeFetchChild(const SilikoNode *parent, int child_index)
{
	if ((child_index = checkBounds(parent, child_index)) < 0)
		return NULL;

	return parent->Branch->Children[child_index];
}

SilikoNode *SilikoNodePruneChild(SilikoNode *parent, int child_index)
{
	if ((child_index = checkBounds(parent, child_index)) < 0)
		return NULL;

	SilikoNode *child = parent->Branch->Children[child_index];
	if (!child)
		return NULL;

	parent->Branch->Count--;
	for (int i = child_index; i < parent->Branch->Count; i++)
		parent->Branch->Children[i] = parent->Branch->Children[i+1];

	parent->Branch->Children[parent->Branch->Count] = NULL;

	return child;
}
