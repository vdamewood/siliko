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

struct SilikoFunction
{
    const struct SilikoFunctionVTable *v_table;
    void *state;
};

SilikoFunction *SilikoFunctionCreate(
	const struct SilikoFunctionVTable *source_table,
	void *source_state)
{
    SilikoFunction *object = malloc(sizeof(*object));
    if (!object)
        return NULL;

    object->v_table = source_table;
    object->state = source_state;
    return object;
}

SilikoValue *SilikoFunctionCall(
    SilikoFunction *object,
    int argc,
    SilikoValue **argv)
{
    if (!object)
        return SilikoValueCreateFromError(SilikoErrorNullObject);
    return object->v_table->call(object->state, argc, argv);
}

void SilikoFunctionDestroy(SilikoFunction *object)
{
    if (object)
        object->v_table->destroy(object->state);
}
