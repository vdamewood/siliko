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

#include <SilikoCore/Function.h>
#include <SilikoCore/Value.h>

struct FunctionState
{
    SilikoValue *(*function)(int argc, SilikoValue **argv);
};

static SilikoValue *Call(void *state, int argc, SilikoValue **argv);
static void Destroy(void *state);
static const struct SilikoFunctionVTable VTable = {
	.call = Call,
	.destroy = Destroy
};

static SilikoValue *Call(void *state, int argc, SilikoValue **argv)
{
    struct FunctionState *typed_state = state;
    return typed_state->function(argc, argv);
}

static void Destroy(void *state)
{
    free(state);
}

SilikoFunction *SilikoPureFunctionCreate(
    SilikoValue *(*source_function)(int argc, SilikoValue **argv))
{
    struct FunctionState *object_state = malloc(sizeof(*object_state));
    if (!object_state)
        return NULL;
    object_state->function = source_function;

    SilikoFunction *object
        = SilikoFunctionCreate(&VTable, object_state);
    if (!object)
    {
        free(object_state);
        return NULL;
    }

    return object;
}
