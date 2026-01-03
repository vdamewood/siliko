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
#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/Value.h>

#if defined __cplusplus
extern "C" {
#endif

enum SilikoNodeStatus
{
	SilikoNodeNothing = 0,
	SilikoNodeBranch,
	SilikoNodeLeaf
};

struct SilikoNode;
typedef struct SilikoNode SilikoNode;

SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewNothing(void);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewFromError(enum SilikoError source);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewFromInteger(long long int source);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewFromReal(double source);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewFromValue(SilikoValue *source);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewBranch(const char *function_Name);
SILIKOCORE_EXPORT SilikoNode *SilikoNodeNewCopy(const SilikoNode *source);
SILIKOCORE_EXPORT void SilikoNodeDelete(SilikoNode *object);

SILIKOCORE_EXPORT void SilikoNodeAssignFromError(SilikoNode *object, enum SilikoError source);
SILIKOCORE_EXPORT void SilikoNodeAssignFromInteger(SilikoNode *object, long long int source);
SILIKOCORE_EXPORT void SilikoNodeAssignFromReal(SilikoNode *object, double source);
SILIKOCORE_EXPORT void SilikoNodeAssignValue(SilikoNode *object, const SilikoValue *source);
SILIKOCORE_EXPORT void SilikoNodeAssignBranch(SilikoNode *object, const char *source);
SILIKOCORE_EXPORT void SilikoNodeCopy(SilikoNode *object, const SilikoNode *source);

SILIKOCORE_EXPORT enum SilikoNodeStatus SilikoNodeGetStatus(const SilikoNode *SyntaxTree);
SILIKOCORE_EXPORT const SilikoValue *SilikoNodeGetValue(const SilikoNode *object);
SILIKOCORE_EXPORT const char *SilikoNodeGetId(const SilikoNode *object);
SILIKOCORE_EXPORT int SilikoNodeIsNegated(const SilikoNode *object);
SILIKOCORE_EXPORT int SilikoNodeNegate(SilikoNode *SyntaxTree);
SILIKOCORE_EXPORT int SilikoNodePushCopyLeft(SilikoNode *base_tree, const SilikoNode *new_branch);
SILIKOCORE_EXPORT int SilikoNodePushLeft(SilikoNode *BaseTree, SilikoNode *NewBranch);
SILIKOCORE_EXPORT int SilikoNodePushCopyRight(SilikoNode *base_tree, const SilikoNode *new_branch);
SILIKOCORE_EXPORT int SilikoNodePushRight(SilikoNode *BaseTree, SilikoNode *NewBranch);
SILIKOCORE_EXPORT int SilikoNodeInsertCopy(SilikoNode *base_tree, int position, const SilikoNode *new_branch);
SILIKOCORE_EXPORT int SilikoNodeInsert(SilikoNode *BaseTree, int position, SilikoNode *NewBranch);
SILIKOCORE_EXPORT int SilikoNodeCountChildren(const SilikoNode *object);
SILIKOCORE_EXPORT const SilikoNode *SilikoNodeFetchChild(const SilikoNode *parent, int childIndex);
SILIKOCORE_EXPORT SilikoNode *SilikoNodePruneChild(SilikoNode *parent, int childIndex);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_NODE_H
