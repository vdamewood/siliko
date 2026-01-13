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


#if !defined SILIKO_CORE_NODE_H
#define SILIKO_CORE_NODE_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Value.h>

enum SilikoNodeStatus
{
	SilikoNodeNothing = 0,
	SilikoNodeBranch,
	SilikoNodeLeaf
};

typedef struct SilikoNode SilikoNode;

SILIKO_EXPORT_C_API

SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateNothing(void);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateFromError(enum SilikoError source);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateFromInteger(long long int source);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateFromReal(double source);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateFromValue(SilikoValue *source);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCreateBranch(const char *function_Name);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodeCopy(const SilikoNode *source);
SILIKOCORE_EXPORT
void SilikoNodeDestroy(SilikoNode *object);

SILIKOCORE_EXPORT
void SilikoNodeAssignFromError(
	SilikoNode *object,
	enum SilikoError source);
SILIKOCORE_EXPORT
void SilikoNodeAssignFromInteger(
	SilikoNode *object,
	long long int source);
SILIKOCORE_EXPORT
void SilikoNodeAssignFromReal(
	SilikoNode *object,
	double source);
SILIKOCORE_EXPORT
void SilikoNodeAssignValue(
	SilikoNode *object,
	const SilikoValue *source);
SILIKOCORE_EXPORT
void SilikoNodeAssignBranch(
	SilikoNode *object,
	const char *source);
SILIKOCORE_EXPORT
void SilikoNodeAssign(
	SilikoNode *object,
	const SilikoNode *source);

SILIKOCORE_EXPORT
int SilikoNodeCountChildren(
	const SilikoNode *object);
SILIKOCORE_EXPORT
const SilikoNode *SilikoNodeFetchChild(
	const SilikoNode *object,
	int child_index);
SILIKOCORE_EXPORT
const char *SilikoNodeGetId(
	const SilikoNode *object);
SILIKOCORE_EXPORT
int SilikoNodeInsertCopy(
	SilikoNode *object,
	int position,
	const SilikoNode *new_branch);
SILIKOCORE_EXPORT
int SilikoNodeInsert(
	SilikoNode *object,
	int position,
	SilikoNode *new_branch);
SILIKOCORE_EXPORT
int SilikoNodeIsNegated(
	const SilikoNode *object);
SILIKOCORE_EXPORT
int SilikoNodeNegate(
	SilikoNode *object);
SILIKOCORE_EXPORT
SilikoNode *SilikoNodePruneChild(
	SilikoNode *object,
	int child_index);
SILIKOCORE_EXPORT
int SilikoNodePushCopyLeft(
	SilikoNode *object,
	const SilikoNode *new_branch);
SILIKOCORE_EXPORT
int SilikoNodePushLeft(
	SilikoNode *object,
	SilikoNode *new_branch);
SILIKOCORE_EXPORT
int SilikoNodePushCopyRight(
	SilikoNode *object,
	const SilikoNode *new_branch);
SILIKOCORE_EXPORT
int SilikoNodePushRight(
	SilikoNode *object,
	SilikoNode *new_branch);
SILIKOCORE_EXPORT
enum SilikoNodeStatus SilikoNodeGetStatus(
	const SilikoNode *object);
SILIKOCORE_EXPORT
const SilikoValue *SilikoNodeGetValue(
	const SilikoNode *object);

SILIKO_END_C_API

#endif // SILIKO_CORE_NODE_H
