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

#include <SilikoCore/Input.h>

struct SilikoInput
{
	const struct SilikoInputVTable *v_table;
	void *state;
};

SilikoInput *SilikoInputCreate(
	const struct SilikoInputVTable *source_table,
	void *source_state)
{
	SilikoInput *object = malloc(sizeof(*object));
	if (object)
	{
		object->v_table = source_table;
		object->state = source_state;
	}
	return object;
}

void SilikoInputDestroy(SilikoInput *object)
{
	if (object)
	{
		object->v_table->destroy(object->state);
		free(object);
	}
}

int SilikoInputAdvance(SilikoInput *object)
{
	if (!object)
		return 0;
	return object->v_table->advance(object->state);
}

char SilikoInputGetCharacter(SilikoInput *object)
{
	if (!object)
		return '\0';
	return object->v_table->getCharacter(object->state);
}
