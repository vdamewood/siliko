/*
 * XaviEvaluate.c: Keep track of dangling objects while composing syntax tree.
 * Copyright 2012 Vincent Damewood
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

#include <stdlib.h>
#include <stdio.h>

#include "XaviArglist.h"
#include "XaviCleanup.h"
#include "XaviFunctionId.h"
#include "XaviTree.h"

struct TreeList {
	struct TreeList * next;
	XaviTree * item;
};

struct ArglistList { // Yeah, I know.
	struct ArglistList * next;
	XaviArglist * item;
};

struct IdList {
	struct IdList * next;
	char * item;
};


static struct TreeList * DanglingTrees = NULL;
static struct ArglistList * DanglingArglists = NULL;
static struct IdList * DanglingIds = NULL;

void XaviCleanupClearAll()
{
	XaviCleanupClearTrees();
	XaviCleanupClearArglists();
	XaviCleanupClearFunctionIds();
}

void XaviCleanupCacheTree(XaviTree * in)
{
	struct TreeList * NewNode;
	struct TreeList * CurrentNode;
	NewNode = malloc(sizeof(struct TreeList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!DanglingTrees) {
		DanglingTrees = NewNode;
	}
	else {
		CurrentNode = DanglingTrees;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseTree(XaviTree * in)
{
	struct TreeList ** ParentPointer;
	struct TreeList * CurrentNode;

	if (!DanglingTrees) {
		return;
	}

	CurrentNode = DanglingTrees;
	ParentPointer = &DanglingTrees;
	
	while (CurrentNode) {
		if (CurrentNode->item == in) {
			*ParentPointer = CurrentNode->next;
			free(CurrentNode);
			CurrentNode = *ParentPointer;
		}
		else {
			ParentPointer = &(CurrentNode->next);
			CurrentNode = *ParentPointer;
		}
	}
}

void XaviCleanupClearTrees()
{
	struct TreeList * CurrentNode;
	struct TreeList * OldNode;

	CurrentNode = DanglingTrees;
	DanglingTrees = NULL;
	
	while (CurrentNode) {
		XaviTreeDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}

//////////////////////////////////////////////////////////////////////////////

void XaviCleanupCacheArglist(XaviArglist * in)
{
	struct ArglistList * NewNode;
	struct ArglistList * CurrentNode;
	NewNode = malloc(sizeof(struct ArglistList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!DanglingArglists) {
		DanglingArglists = NewNode;
	}
	else {
		CurrentNode = DanglingArglists;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseArglist(XaviArglist * in)
{
	struct ArglistList ** ParentPointer;
	struct ArglistList * CurrentNode;

	if (!DanglingArglists) {
		return;
	}

	CurrentNode = DanglingArglists;
	ParentPointer = &DanglingArglists;
	
	while (CurrentNode) {
		if (CurrentNode->item == in) {
			*ParentPointer = CurrentNode->next;
			free(CurrentNode);
			CurrentNode = *ParentPointer;
		}
		else {
			ParentPointer = &(CurrentNode->next);
			CurrentNode = *ParentPointer;
		}
	}
}

void XaviCleanupClearArglists()
{
	struct ArglistList * CurrentNode;
	struct ArglistList * OldNode;

	CurrentNode = DanglingArglists;
	DanglingArglists = NULL;
	
	while (CurrentNode) {
		XaviArglistDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}

//////////////////////////////////////////////////////////////////////////////

void XaviCleanupCacheFunctionId(char * in)
{
	struct IdList * NewNode;
	struct IdList * CurrentNode;
	NewNode = malloc(sizeof(struct IdList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!DanglingIds) {
		DanglingIds = NewNode;
	}
	else {
		CurrentNode = DanglingIds;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseFunctionId(char * in)
{
	struct IdList ** ParentPointer;
	struct IdList * CurrentNode;

	if (!DanglingIds) {
		return;
	}

	CurrentNode = DanglingIds;
	ParentPointer = &DanglingIds;
	
	while (CurrentNode) {
		if (CurrentNode->item == in) {
			*ParentPointer = CurrentNode->next;
			free(CurrentNode);
			CurrentNode = *ParentPointer;
		}
		else {
			ParentPointer = &(CurrentNode->next);
			CurrentNode = *ParentPointer;
		}
	}
}

void XaviCleanupClearFunctionIds()
{
	struct IdList * CurrentNode;
	struct IdList * OldNode;

	CurrentNode = DanglingIds;
	DanglingIds = NULL;
	
	while (CurrentNode) {
		XaviFunctionIdDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}
