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
#include <stdint.h>
#include <string.h>

#include <SilikoCore/Engine.h>
#include <SilikoCore/Function.h>
#include <SilikoCore/Value.h>

#if defined _MSC_VER
#define strdup _strdup
#endif

struct SilikoFunctionChain
{
	char *id;
	SilikoFunction *function;
	struct SilikoFunctionChain *next;
};

#define TableSize 127

static uint8_t hash(const char *input)
{
	uint16_t remainder = 0;

	for (const uint8_t *i = (const uint8_t *)input; *i; i++)
        remainder = ((remainder << 1) + *i) % TableSize;

	return remainder;
}

struct SilikoEngine
{
	struct SilikoFunctionChain *table[TableSize];
};

SilikoEngine *SilikoEngineCreate(void)
{
	return calloc(1, sizeof(SilikoEngine));
}

void SilikoEngineDestroy(SilikoEngine *object)
{
	if (!object)
		return;

	for (
		size_t i = 0;
		i < sizeof(object->table)/sizeof(object->table[0]);
		i++)
	{
		if (object->table[i])
		{
			struct SilikoFunctionChain *current = object->table[i];
			while (current)
			{
				struct SilikoFunctionChain *next = current->next;
				free(current->id);
				free(current);
				current = next;
			}
		}
	}

	free(object);
}

int SilikoEngineInstallFunction(
	SilikoEngine *object,
	const char *name,
	SilikoFunction *function)
{
	if (!object)
		return 0;

	uint8_t index = hash(name);
	struct SilikoFunctionChain  *new_node = malloc(sizeof *new_node);
	if (!new_node)
		return 0;

	new_node->id = strdup(name);
	new_node->function = function;
	new_node->next = NULL;

	if (object->table[index])
	{
		struct SilikoFunctionChain *current_node
			= object->table[index];

		while (current_node->next)
			current_node = current_node->next;

		current_node->next = new_node;
	}
	else
	{
		object->table[index] = new_node;
	}

	return -1;
}

SilikoFunction *SilikoEngineFetchFunction(
	SilikoEngine *object,
	const char *name)
{
	if (!object)
		return NULL;

	uint8_t index = hash(name);
	struct SilikoFunctionChain *current = object->table[index];

	while (current)
		if (strcmp(name, current->id) != 0)
			current = current->next;
		else
			break;

	if (current)
		return current->function;
	else
		return NULL;
}

SilikoValue *SilikoEngineCallFunction(
	SilikoEngine *object,
	const char *name,
	int argc,
	SilikoValue **argv)
{
	if (!object)
		return SilikoValueCreateFromError(SilikoErrorNullObject);

	SilikoFunction *function
		= SilikoEngineFetchFunction(object, name);

	if (!function)
		return SilikoValueCreateFromError(SilikoErrorFunctionName);

	return SilikoFunctionCall(function, argc, argv);
}
