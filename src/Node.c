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
#include <SilikoCore/Value.h>

#if defined _MSC_VER
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
		SilikoNodeDestroy(branch->Children[i]);
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

static inline int OutOfBounds(const SilikoNode *parent, size_t child_index)
{
	return parent->Type != SilikoNodeBranch
			|| child_index >= parent->Branch->Count;
}

SilikoNode *SilikoNodeCreateNothing(void)
{
	SilikoNode *rVal = NULL;

	if ((rVal = malloc(sizeof(SilikoNode))))
		rVal->Type = SilikoNodeNothing;

	return rVal;
}

SilikoNode *SilikoNodeCreateFromError(enum SilikoError source)
{
	SilikoNode *object = malloc(sizeof(SilikoNode));
	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueCreateFromError(source);
	if(!object->Leaf)
	{
		free (object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeCreateFromInteger(long long int source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueCreateFromInteger(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeCreateFromReal(double source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueCreateFromReal(source);
	if(!object->Leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeCreateFromValue(SilikoValue *source)
{
	SilikoNode *object
		= malloc(sizeof(*object));

	if (!object)
		return NULL;

	object->Type = SilikoNodeLeaf;
	object->Leaf = SilikoValueCopy(source);
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

SilikoNode *SilikoNodeCreateBranch(const char *NewId)
{
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

SilikoNode *SilikoNodeCopy(const SilikoNode *source)
{
	SilikoNode *object = SilikoNodeCreateNothing();
	SilikoNodeAssign(object, source);
	return object;
}

void SilikoNodeAssignFromError(SilikoNode *object, enum SilikoError source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromError(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignFromInteger(SilikoNode *object, long long int source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromInteger(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignFromReal(SilikoNode *object, double source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromReal(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}


void SilikoNodeAssignValue(SilikoNode *object, const SilikoValue *source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCopy(source);
	if (!new_leaf)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeLeaf;
	object->Leaf = new_leaf;
}

void SilikoNodeAssignBranch(SilikoNode *object, const char *source)
{
	if (!object)
		return;

	struct Branch *new_branch = NewBranch(source);
	if (!new_branch)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = SilikoNodeBranch;
	object->Branch = new_branch;
}

void SilikoNodeAssign(SilikoNode *object, const SilikoNode *source)
{
	if (!object || object == source)
		return;

	if (object->Type == SilikoNodeBranch)
		DeleteBranch(object->Branch);

	object->Type = source->Type;
	switch (object->Type)
	{
	case SilikoNodeLeaf:
		object->Leaf = SilikoValueCopy(source->Leaf);
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
				= SilikoNodeCopy(source->Branch->Children[i]);
		}
		break;
	}
	default:
		;
	}
}

static int ExpandChildren(SilikoNode *Tree)
{
	int new_capacity = Tree->Branch->Capacity * 2;
	SilikoNode **new_children =
		calloc(new_capacity, sizeof *new_children);

	if (!new_children)
		return 0;

	memcpy(
		new_children,
		Tree->Branch->Children,
		Tree->Branch->Capacity * sizeof *new_children);
	free(Tree->Branch->Children);

	Tree->Branch->Children = new_children;
	Tree->Branch->Capacity = new_capacity;
	return -1;
}

int SilikoNodePushCopyLeft(SilikoNode *object, const SilikoNode *new_child)
{
	if (!object || !new_child)
		return 0;

	SilikoNode *new_child_copy = SilikoNodeCopy(new_child);
	if (!new_child_copy)
		return 0;

	if (!SilikoNodePushLeft(object, new_child_copy))
	{
		SilikoNodeDestroy(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodePushLeft(SilikoNode *object, SilikoNode *new_child)
{
	if (!object || !new_child)
		return 0;

	if (object->Branch->Count == object->Branch->Capacity)
		if (!ExpandChildren(object))
			return 0;

	for (int i = object->Branch->Count; i >= 1; i--)
		object->Branch->Children[i] = object->Branch->Children[i-1];

	object->Branch->Children[0] = new_child;
	object->Branch->Count++;
	return -1;
}

int SilikoNodePushCopyRight(SilikoNode *object, const SilikoNode *new_child)
{
	if (!object || !new_child)
		return 0;

	SilikoNode *new_child_copy = SilikoNodeCopy(new_child);
	if (!new_child_copy)
		return 0;

	if (!SilikoNodePushRight(object, new_child_copy))
	{
		SilikoNodeDestroy(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodePushRight(SilikoNode *object, SilikoNode *new_child)
{
	if (!object || !new_child)
		return 0;

	if (object->Branch->Count == object->Branch->Capacity)
		if (!ExpandChildren(object))
			return 0;

	object->Branch->Children[object->Branch->Count] = new_child;
	object->Branch->Count++;
	return -1;
}

int SilikoNodeInsertCopy(SilikoNode *object, size_t position, const SilikoNode *new_child)
{
	if (!object || !new_child)
		return 0;

	SilikoNode *new_child_copy = SilikoNodeCopy(new_child);
	if (!new_child_copy)
		return 0;

	if (!SilikoNodeInsert(object, position, new_child_copy))
	{
		SilikoNodeDestroy(new_child_copy);
		return 0;
	}

	return -1;
}

int SilikoNodeInsert(
	SilikoNode *object,
	size_t position,
	SilikoNode *new_child)
{
	if (!object || !new_child || OutOfBounds(object, position))
		return 0;

	if (object->Branch->Count == object->Branch->Capacity)
		if (!ExpandChildren(object))
			return 0;

	for (int i = object->Branch->Count; i > position; i--)
		object->Branch->Children[i] = object->Branch->Children[i-1];

	object->Branch->Children[position] = new_child;
	object->Branch->Count++;
	return -1;
}

enum SilikoNodeStatus SilikoNodeGetStatus(const SilikoNode *object)
{
	if (!object)
		return SilikoNodeNothing;

	return object->Type;
}

const SilikoValue *SilikoNodeGetValue(const SilikoNode *object)
{
	if (!object)
		return SilikoValueCreateFromError(SilikoErrorNullObject);

	return object->Type == SilikoNodeLeaf
		? object->Leaf
		: NULL;
}

const char *SilikoNodeGetId(const SilikoNode *object)
{
	if (!object || object->Type != SilikoNodeBranch)
		return NULL;

	return object->Branch->Id;
}

int SilikoNodeIsNegated(const SilikoNode *object)
{
	if (!object)
		return 0;

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

void SilikoNodeDestroy(SilikoNode *Node)
{
	if (Node)
	{
		if (Node->Type == SilikoNodeBranch)
		DeleteBranch(Node->Branch);
		free(Node);
	}
}

int SilikoNodeCountChildren(const SilikoNode *object)
{
	if (!object || object->Type != SilikoNodeBranch)
		return 0;

	return object->Branch->Count;
}

const SilikoNode *SilikoNodeFetchChild(
	const SilikoNode *parent,
	size_t child_index)
{
	if (!parent || OutOfBounds(parent, child_index))
		return NULL;

	return parent->Branch->Children[child_index];
}

SilikoNode *SilikoNodePruneChild(SilikoNode *parent, size_t child_index)
{
	if (!parent || OutOfBounds(parent, child_index))
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
