/*
 * StringSource.c: Support for reading from a string
 * Copyright 2012, 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "StringSource.h"

#if defined WIN32
#define strdup _strdup
#endif

struct XaviStringSourceState
{
	char *string;
	char *current;
};

typedef struct XaviStringSourceState XaviStringSourceState;

static int XaviStringSourceAdvance(void *State)
{
	if (((XaviStringSourceState *)State)->current)
	{
		((XaviStringSourceState *)State)->current++;
		return -1;
	}
	else
	{
		return 0;
	}
}

static char XaviStringSourceGet(void *State)
{
	return *((XaviStringSourceState *)State)->current;
}

static void XaviStringSourceDestroy(void *State)
{
	if (State)
		free(((XaviStringSourceState *)State)->string);
	free(State);
}

XaviDataSource *XaviStringSourceNew(const char *NewInput)
{
	XaviStringSourceState *state = NULL;
	XaviDataSource *source = NULL;

	if (!(state = malloc(sizeof(XaviStringSourceState))))
		goto memerr;

	// This line protects against freeing the undefined pointer
	// that string would be set to if it weren't set to NULL.
	state->string = NULL;

	state->string = strdup(NewInput);

	state->current = state->string;

	if (!(source = malloc(sizeof(XaviDataSource))))
		goto memerr;

	source->State = state;
	source->AdvanceFunction = XaviStringSourceAdvance;
	source->GetFunction = XaviStringSourceGet;
	source->DestroyFunction = XaviStringSourceDestroy;

	return source;
memerr:
	free(source);
	if (state)
		free(state->string);
	free(state);
	return NULL;
}
