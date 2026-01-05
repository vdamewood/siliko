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


#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <SilikoCore/FunctionCaller.h>
#include <SilikoCore/Function.h>
#include <SilikoCore/Value.h>

#include "Functions.h"

#if defined _WIN32
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

struct SilikoFunctionCaller
{
	struct SilikoFunctionChain *table[TableSize];
};

SilikoFunctionCaller *SilikoFunctionCallerNew()
{
	return calloc(1, sizeof(SilikoFunctionCaller));
}

void SilikoFunctionCallerDelete(SilikoFunctionCaller *Caller)
{
	for (size_t i = 0; i < sizeof(Caller->table)/sizeof(Caller->table[0]); i++)
	{
		if (Caller->table[i])
		{
			struct SilikoFunctionChain *current = Caller->table[i];
			while (current)
			{
				struct SilikoFunctionChain *next = current->next;
				free(current->id);
				free(current);
				current = next;
			}
		}
	}

	free(Caller);
}

int SilikoFunctionCallerInstall(
	SilikoFunctionCaller *Caller,
	const char *name,
	SilikoFunction *function)
{
	uint8_t bucket = hash(name);
	struct SilikoFunctionChain  *newNode;

	if (!(newNode = malloc(sizeof(struct SilikoFunctionChain))))
		return 0;

	newNode->id = strdup(name);
	newNode->function = function;
	newNode->next = NULL;

	if (Caller->table[bucket])
	{
		struct SilikoFunctionChain *currentNode = Caller->table[bucket];
		while (currentNode->next)
			currentNode = currentNode->next;
		currentNode->next = newNode;
	}
	else
	{
		Caller->table[bucket] = newNode;
	}

	return -1;
}

int SilikoFunctionCallerInstallOperators(SilikoFunctionCaller *Caller)
{
	return SilikoFunctionCallerInstall(Caller, "add",
			SilikoPureFunctionNew(SilikoFunction_add))
		&& SilikoFunctionCallerInstall(Caller, "subtract",
			SilikoPureFunctionNew(SilikoFunction_subtract))
		&& SilikoFunctionCallerInstall(Caller, "multiply",
			SilikoPureFunctionNew(SilikoFunction_multiply))
		&& SilikoFunctionCallerInstall(Caller, "divide",
			SilikoPureFunctionNew(SilikoFunction_divide))
		&& SilikoFunctionCallerInstall(Caller, "power",
			SilikoPureFunctionNew(SilikoFunction_power))
		&& SilikoFunctionCallerInstall(Caller, "dice",
			SilikoPureFunctionNew(SilikoFunction_dice));
}

int SilikoFunctionCallerInstallFunctions(SilikoFunctionCaller *Caller)
{
	return SilikoFunctionCallerInstall(Caller, "abs",
			SilikoPureFunctionNew(SilikoFunction_abs))
		&& SilikoFunctionCallerInstall(Caller, "acos",
			SilikoPureFunctionNew(SilikoFunction_acos))
		&& SilikoFunctionCallerInstall(Caller, "asin",
			SilikoPureFunctionNew(SilikoFunction_asin))
		&& SilikoFunctionCallerInstall(Caller, "atan",
			SilikoPureFunctionNew(SilikoFunction_atan))
		&& SilikoFunctionCallerInstall(Caller, "ceil",
			SilikoPureFunctionNew(SilikoFunction_ceil))
		&& SilikoFunctionCallerInstall(Caller, "cos",
			SilikoPureFunctionNew(SilikoFunction_cos))
		&& SilikoFunctionCallerInstall(Caller, "cosh",
			SilikoPureFunctionNew(SilikoFunction_cosh))
		&& SilikoFunctionCallerInstall(Caller, "exp",
			SilikoPureFunctionNew(SilikoFunction_exp))
		&& SilikoFunctionCallerInstall(Caller, "floor",
			SilikoPureFunctionNew(SilikoFunction_floor))
		&& SilikoFunctionCallerInstall(Caller, "log",
			SilikoPureFunctionNew(SilikoFunction_log))
		&& SilikoFunctionCallerInstall(Caller, "log10",
			SilikoPureFunctionNew(SilikoFunction_log10))
		&& SilikoFunctionCallerInstall(Caller, "sin",
			SilikoPureFunctionNew(SilikoFunction_sin))
		&& SilikoFunctionCallerInstall(Caller, "sinh",
			SilikoPureFunctionNew(SilikoFunction_sinh))
		&& SilikoFunctionCallerInstall(Caller, "sqrt",
			SilikoPureFunctionNew(SilikoFunction_sqrt))
		&& SilikoFunctionCallerInstall(Caller, "tan",
			SilikoPureFunctionNew(SilikoFunction_tan))
		&& SilikoFunctionCallerInstall(Caller, "tanh",
			SilikoPureFunctionNew(SilikoFunction_tanh));
}

SilikoFunction *SilikoFunctionCallerGetFunction(SilikoFunctionCaller *Caller, const char *name)
{
	uint8_t index = hash(name);
	struct SilikoFunctionChain *current = Caller->table[index];

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

SilikoValue *SilikoFunctionCallerCall(
	SilikoFunctionCaller *caller,
	const char *name,
	int argc,
	SilikoValue **argv)
{
	SilikoFunction *function
		= SilikoFunctionCallerGetFunction(caller, name);

	if (!function)
		return SilikoValueNewFromError(SilikoErrorFunctionName);
	
	return SilikoFunctionCall(function, argc, argv);
}
