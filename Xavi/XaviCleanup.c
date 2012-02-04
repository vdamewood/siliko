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

void XaviCleanupClearAll(XaviMemoryPool * pool)
{
	XaviCleanupClearTrees(pool);
	XaviCleanupClearArglists(pool);
	XaviCleanupClearFunctionIds(pool);
}

void XaviCleanupCacheTree(XaviTree * in, XaviMemoryPool * pool)
{
	struct XaviTreeList * NewNode;
	struct XaviTreeList * CurrentNode;
	NewNode = malloc(sizeof(XaviTreeList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!pool->DanglingTrees) {
		pool->DanglingTrees = NewNode;
	}
	else {
		CurrentNode = pool->DanglingTrees;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseTree(XaviTree * in, XaviMemoryPool * pool)
{
	struct XaviTreeList ** ParentPointer;
	struct XaviTreeList * CurrentNode;

	if (!pool->DanglingTrees) {
		return;
	}

	CurrentNode = pool->DanglingTrees;
	ParentPointer = &(pool->DanglingTrees);
	
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

void XaviCleanupClearTrees(XaviMemoryPool * pool)
{
	struct XaviTreeList * CurrentNode;
	struct XaviTreeList * OldNode;

	CurrentNode = pool->DanglingTrees;
	pool->DanglingTrees = NULL;
	
	while (CurrentNode) {
		XaviTreeDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}

//////////////////////////////////////////////////////////////////////////////

void XaviCleanupCacheArglist(XaviArglist * in, XaviMemoryPool * pool)
{
	struct XaviArglistList * NewNode;
	struct XaviArglistList * CurrentNode;
	NewNode = malloc(sizeof(XaviArglistList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!pool->DanglingArglists) {
		pool->DanglingArglists = NewNode;
	}
	else {
		CurrentNode = pool->DanglingArglists;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseArglist(XaviArglist * in, XaviMemoryPool * pool)
{
	struct XaviArglistList ** ParentPointer;
	struct XaviArglistList * CurrentNode;

	if (!(pool->DanglingArglists)) {
		return;
	}

	CurrentNode = pool->DanglingArglists;
	ParentPointer = &(pool->DanglingArglists);
	
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

void XaviCleanupClearArglists(XaviMemoryPool * pool)
{
	struct XaviArglistList * CurrentNode;
	struct XaviArglistList * OldNode;

	CurrentNode = pool->DanglingArglists;
	pool->DanglingArglists = NULL;
	
	while (CurrentNode) {
		XaviArglistDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}

//////////////////////////////////////////////////////////////////////////////

void XaviCleanupCacheFunctionId(char * in, XaviMemoryPool * pool)
{
	struct XaviIdList * NewNode;
	struct XaviIdList * CurrentNode;
	NewNode = malloc(sizeof(XaviIdList));
	NewNode->item = in;
	NewNode->next = NULL;

	if (!(pool->DanglingIds)) {
		pool->DanglingIds = NewNode;
	}
	else {
		CurrentNode = pool->DanglingIds;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
	}
}

void XaviCleanupReleaseFunctionId(char * in, XaviMemoryPool * pool)
{
	struct XaviIdList ** ParentPointer;
	struct XaviIdList * CurrentNode;

	if (!(pool->DanglingIds)) {
		return;
	}

	CurrentNode = pool->DanglingIds;
	ParentPointer = &(pool->DanglingIds);
	
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

void XaviCleanupClearFunctionIds(XaviMemoryPool * pool)
{
	struct XaviIdList * CurrentNode;
	struct XaviIdList * OldNode;

	CurrentNode = pool->DanglingIds;
	pool->DanglingIds = NULL;
	
	while (CurrentNode) {
		XaviFunctionIdDelete(CurrentNode->item);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}
