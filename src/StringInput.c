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

#include <SilikoCore/Input.h>

#if defined _MSC_VER
#define strdup _strdup
#endif

struct StringInputState
{
	char *string;
	char *current;
};

static bool Advance(void *state)
{
	struct StringInputState *typed_state = state;

	if (typed_state->current)
	{
		typed_state->current++;
		return true;
	}
	else
	{
		return false;
	}
}

static char GetCharacter(void *state)
{
	struct StringInputState *typed_state = state;
	return *typed_state->current;
}

static void Destroy(void *state)
{
	if (state)
	{
		struct StringInputState *typed_state = state;
		free(typed_state->string);
	}
	free(state);
}

static const struct SilikoInputVTable vTable = {
	.advance = Advance,
	.getCharacter = GetCharacter,
	.destroy = Destroy
};

SilikoInput *SilikoStringInputCreate(const char *source)
{
	struct StringInputState *state = malloc(sizeof *state);
	if (!state)
		return NULL;

	if (!(state->string = strdup(source)))
	{
		free(state);
		return NULL;
	}
	state->current = state->string;

	SilikoInput *object = SilikoInputCreate(&vTable, state);
	if (!object)
	{
		free(state->string);
		free(state);
		return NULL;
	}

	return object;
}
