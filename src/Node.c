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
	char *id;
	size_t count;
	size_t capacity;
	int isNegated;
	SilikoNode **children;
};

static void DeleteBranch(struct Branch *branch)
{
	for (int i = 0; i < branch->count; i++)
		SilikoNodeDestroy(branch->children[i]);
	free(branch->id);
	free(branch->children);
	free(branch);
}

struct SilikoNode
{
	enum SilikoNodeStatus status;
	union
	{
		SilikoValue *leaf;
		struct Branch *branch;
	};
};

static inline int OutOfBounds(
	const SilikoNode *parent,
	size_t child_index)
{
	return parent->status != SilikoNodeBranch
			|| child_index >= parent->branch->count;
}

SilikoNode *SilikoNodeCreateNothing(void)
{
	SilikoNode *object = malloc(sizeof *object);

	if (object)
		object->status = SilikoNodeNothing;

	return object;
}

SilikoNode *SilikoNodeCreateFromError(enum SilikoError source)
{
	SilikoNode *object = malloc(sizeof *object);
	if (!object)
		return NULL;

	object->status = SilikoNodeLeaf;
	object->leaf = SilikoValueCreateFromError(source);
	if(!object->leaf)
	{
		free (object);
		return NULL;
	}

	return object;
}

SilikoNode *SilikoNodeCreateFromInteger(long long int source)
{
	SilikoNode *object
		= malloc(sizeof *object);
	if (!object)
		return NULL;

	object->status = SilikoNodeLeaf;
	object->leaf = SilikoValueCreateFromInteger(source);
	if(!object->leaf)
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

	object->status = SilikoNodeLeaf;
	object->leaf = SilikoValueCreateFromReal(source);
	if(!object->leaf)
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

	object->status = SilikoNodeLeaf;
	object->leaf = SilikoValueCopy(source);
	if(!object->leaf)
	{
		free(object);
		return NULL;
	}

	return object;
}

static struct Branch *NewBranch(const char* source_id)
{
	const int default_size = 4;
	struct Branch *object = malloc(sizeof *object);
	SilikoNode **new_children = calloc(default_size, sizeof *new_children);
	char *new_id = strdup(source_id);
	if (!object || !new_children || !new_id)
	{
		free(object);
		free(new_children);
		free(new_id);
		return NULL;
	}

	object->id = new_id;
	object->count = 0;
	object->isNegated = 0;
	object->capacity = default_size;
	object->children = new_children;

	return object;
}

SilikoNode *SilikoNodeCreateBranch(const char *source_id)
{
	SilikoNode *object = malloc(sizeof *object);
	if (!object)
		return NULL;

	struct Branch *new_branch = NewBranch(source_id);
	if (!new_branch)
	{
		free(object);
		return NULL;
	}

	object->status = SilikoNodeBranch;
	object->branch = new_branch;
	return object;
}

SilikoNode *SilikoNodeCopy(const SilikoNode *source)
{
	SilikoNode *object = SilikoNodeCreateNothing();
	SilikoNodeAssign(object, source);
	return object;
}

void SilikoNodeDestroy(SilikoNode *object)
{
	if (object)
	{
		if (object->status == SilikoNodeLeaf)
			SilikoValueDestroy(object->leaf);

		if (object->status == SilikoNodeBranch)
			DeleteBranch(object->branch);

		free(object);
	}
}

void SilikoNodeAssignFromError(
	SilikoNode *object,
	enum SilikoError source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromError(source);
	if (!new_leaf)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = SilikoNodeLeaf;
	object->leaf = new_leaf;
}

void SilikoNodeAssignFromInteger(
	SilikoNode *object,
	long long int source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromInteger(source);
	if (!new_leaf)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = SilikoNodeLeaf;
	object->leaf = new_leaf;
}

void SilikoNodeAssignFromReal(SilikoNode *object, double source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCreateFromReal(source);
	if (!new_leaf)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = SilikoNodeLeaf;
	object->leaf = new_leaf;
}

void SilikoNodeAssignFromValue(
	SilikoNode *object,
	const SilikoValue *source)
{
	if (!object)
		return;

	SilikoValue *new_leaf = SilikoValueCopy(source);
	if (!new_leaf)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = SilikoNodeLeaf;
	object->leaf = new_leaf;
}

void SilikoNodeAssignBranch(SilikoNode *object, const char *source)
{
	if (!object)
		return;

	struct Branch *new_branch = NewBranch(source);
	if (!new_branch)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = SilikoNodeBranch;
	object->branch = new_branch;
}

void SilikoNodeAssign(SilikoNode *object, const SilikoNode *source)
{
	if (!object || object == source)
		return;

	if (object->status == SilikoNodeBranch)
		DeleteBranch(object->branch);

	object->status = source->status;
	switch (object->status)
	{
	case SilikoNodeLeaf:
		object->leaf = SilikoValueCopy(source->leaf);
		if (!object->leaf)
		{
			free(object);
			object = NULL;
			return;
		}
		break;
	case SilikoNodeBranch:
	{
		object->branch = malloc(sizeof(*object->branch));
		if (!object->branch)
		{
			free(object);
			object = NULL;
			return;
		}
		object->branch->id = strdup(source->branch->id);
		if (!object->branch->id)
		{
			free(object->branch);
			free(object);
			object = NULL;
			return;
		}
		object->branch->count = source->branch->count;
		object->branch->capacity = source->branch->capacity;
		object->branch->isNegated = source->branch->isNegated;
		object->branch->children = calloc(
			object->branch->capacity,
			sizeof(*object->branch->children));
		if (!object->branch->children)
		{
			free(object->branch->id);
			free(object->branch);
			free(object);
			object = NULL;
			return;
		}
		for (int i = 0; i <= object->branch->count; i++)
		{
			object->branch->children[i]
				= SilikoNodeCopy(source->branch->children[i]);
		}
		break;
	}
	default:
		; // Do nothing. Silence warning.
	}
}

size_t SilikoNodeCountChildren(const SilikoNode *object)
{
	if (!object || object->status != SilikoNodeBranch)
		return 0;

	return object->branch->count;
}

const SilikoNode *SilikoNodeFetchChild(
	const SilikoNode *parent,
	size_t child_index)
{
	if (!parent || OutOfBounds(parent, child_index))
		return NULL;

	return parent->branch->children[child_index];
}

const char *SilikoNodeGetId(const SilikoNode *object)
{
	if (!object || object->status != SilikoNodeBranch)
		return NULL;

	return object->branch->id;
}

static int ExpandChildren(SilikoNode *object)
{
	int new_capacity = object->branch->capacity * 2;
	SilikoNode **new_children =
		calloc(new_capacity, sizeof *new_children);

	if (!new_children)
		return 0;

	memcpy(
		new_children,
		object->branch->children,
		object->branch->capacity * sizeof *new_children);
	free(object->branch->children);

	object->branch->children = new_children;
	object->branch->capacity = new_capacity;
	return -1;
}

int SilikoNodeInsertCopy(
	SilikoNode *object,
	size_t position,
	const SilikoNode *new_child)
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

	if (object->branch->count == object->branch->capacity)
		if (!ExpandChildren(object))
			return 0;

	for (int i = object->branch->count; i > position; i--)
		object->branch->children[i] = object->branch->children[i-1];

	object->branch->children[position] = new_child;
	object->branch->count++;
	return -1;
}

int SilikoNodeIsNegated(const SilikoNode *object)
{
	if (!object)
		return 0;

	switch (object->status)
	{
	case SilikoNodeLeaf:
		switch (SilikoValueGetStatus(object->leaf))
		{
		case SilikoValueInteger:
			return SilikoValueGetInteger(object->leaf) < 0;
		case SilikoValueReal:
			return SilikoValueGetReal(object->leaf) < 0.0;
		default:
			return 0;
		}
	case SilikoNodeBranch:
		return object->branch->isNegated;
	default:
		return 0;
	}
}

void SilikoNodeNegate(SilikoNode *object)
{
	if (!object)
		return;

	switch (object->status)
	{
	case SilikoNodeLeaf:
		SilikoValueNegate(object->leaf);
		break;
	case SilikoNodeBranch:
		object->branch->isNegated = !object->branch->isNegated;
		break;
	default:
		; // Do nothing. Silence warning.
	}
}

SilikoNode *SilikoNodePruneChild(SilikoNode *parent, size_t child_index)
{
	if (!parent || OutOfBounds(parent, child_index))
		return NULL;

	SilikoNode *child = parent->branch->children[child_index];
	if (!child)
		return NULL;

	parent->branch->count--;
	for (int i = child_index; i < parent->branch->count; i++)
		parent->branch->children[i] = parent->branch->children[i+1];

	parent->branch->children[parent->branch->count] = NULL;

	return child;
}

int SilikoNodePushCopyLeft(
	SilikoNode *object,
	const SilikoNode *new_child)
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

	if (object->branch->count == object->branch->capacity)
		if (!ExpandChildren(object))
			return 0;

	for (int i = object->branch->count; i >= 1; i--)
		object->branch->children[i] = object->branch->children[i-1];

	object->branch->children[0] = new_child;
	object->branch->count++;
	return -1;
}

int SilikoNodePushCopyRight(
	SilikoNode *object,
	const SilikoNode *new_child)
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

	if (object->branch->count == object->branch->capacity)
		if (!ExpandChildren(object))
			return 0;

	object->branch->children[object->branch->count] = new_child;
	object->branch->count++;
	return -1;
}

enum SilikoNodeStatus SilikoNodeGetStatus(const SilikoNode *object)
{
	if (!object)
		return SilikoNodeNothing;

	return object->status;
}

const SilikoValue *SilikoNodeGetValue(const SilikoNode *object)
{
	if (!object)
		return SilikoValueCreateFromError(SilikoErrorNullObject);

	return object->status == SilikoNodeLeaf
		? object->leaf
		: NULL;
}
