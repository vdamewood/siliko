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


#if !defined SILIKO_CORE_FUNCTION_H
#define SILIKO_CORE_FUNCTION_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Value.h>

typedef struct SilikoFunction SilikoFunction;

SILIKO_EXPORT_C_API

struct SilikoFunctionVTable
{
	SilikoValue *(*callVirt)(void *state, int argc, SilikoValue **argv);
    SilikoFunction *(*cloneVirt)(void *state);
	void (*deleteVirt)(void *state);
};

SILIKOCORE_EXPORT
SilikoFunction *SilikoFunctionCreate(
	const struct SilikoFunctionVTable *source_table,
	void *source_state);

SILIKOCORE_EXPORT
SilikoFunction *SilikoPureFunctionCreate(
    SilikoValue *(*function)(int argc, SilikoValue **argv)
);

SILIKOCORE_EXPORT
SilikoValue *SilikoFunctionCall(
    SilikoFunction *object,
    int argc,
    SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoFunction *SilikoFunctionClone(
    SilikoFunction *object);
SILIKOCORE_EXPORT
void SilikoFunctionDestroy(
    SilikoFunction *object);

SILIKO_END_C_API

#endif // SILIKO_CORE_FUNCTION_H
