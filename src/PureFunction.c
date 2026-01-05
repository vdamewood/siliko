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

struct State
{
    SilikoValue *(*function)(int argc, SilikoValue **argv);
};

static SilikoValue *Call(void *object, int argc, SilikoValue **argv);
static SilikoFunction *Clone(void *void_state);
static void Delete(void *state);
static const struct SilikoFunctionVTable VTable = {
	.callVirt = Call,
	.cloneVirt = Clone,
	.deleteVirt = Delete
};

static SilikoValue *Call(void *void_state, int argc, SilikoValue **argv)
{
    struct State *state = void_state;
    return state->function(argc, argv);
}

static SilikoFunction *Clone(void *void_state)
{
    struct State *source_state = void_state;
    struct State *clone_state = malloc(sizeof *clone_state);
    if (!clone_state)
        return NULL;
    clone_state->function = source_state->function;

    SilikoFunction *clone = SilikoFunctionNew(&VTable, clone_state);
    if (!clone)
    {
        free(clone_state);
        return NULL;
    }

    return clone;
}

static void Delete(void *state)
{
    free(state);
}

SilikoFunction *SilikoPureFunctionNew(
    SilikoValue *(*source_function)(int argc, SilikoValue **argv))
{
    struct State *object_state = malloc(sizeof(*object_state));
    if (!object_state)
        return NULL;
    object_state->function = source_function;

    SilikoFunction *object = SilikoFunctionNew(&VTable, object_state);
    if (!object)
    {
        free(object_state);
        return NULL;
    }

    return object;
}
