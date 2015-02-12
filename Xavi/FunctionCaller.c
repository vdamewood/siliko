/*
 * FunctionCaller.c: Function handling
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

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Hash.h"
#include "Value.h"
#include "Functions.h"
#include "FunctionCaller.h"

#if defined _WIN32
#define strdup _strdup
#endif

struct XaviFunctionChain
{
	char *id;
	XaviFunctionPointer function;
	struct XaviFunctionChain *next;
};
typedef struct XaviFunctionChain XaviFunctionChain;

static XaviFunctionChain **functionTable;

int XaviFunctionCallerInstall(const char *name, XaviFunctionPointer function)
{
	int bucket = XaviHash((const unsigned char *)name, strlen(name));
	XaviFunctionChain  *newNode;

	if (!(newNode = malloc(sizeof(XaviFunctionChain))))
		return 0;

	newNode->id = strdup(name);
	newNode->function = function;
	newNode->next = NULL;

	if (functionTable[bucket])
	{
		XaviFunctionChain *currentNode = functionTable[bucket];
		while (currentNode->next)
			currentNode = currentNode->next;
		currentNode->next = newNode;
	}
	else
	{
		functionTable[bucket] = newNode;
	}

	return -1;
}

int XaviFunctionCallerSetUp()
{
	if (!(functionTable = calloc(256, sizeof(XaviFunctionChain))))
		goto memerr;

	XaviFunctionCallerInstall("add", XaviFunction_add);
	XaviFunctionCallerInstall("subtract", XaviFunction_subtract);
	XaviFunctionCallerInstall("multiply", XaviFunction_multiply);
	XaviFunctionCallerInstall("divide", XaviFunction_divide);
	XaviFunctionCallerInstall("power", XaviFunction_power);
	XaviFunctionCallerInstall("dice", XaviFunction_dice);
	XaviFunctionCallerInstall("abs", XaviFunction_abs);
	XaviFunctionCallerInstall("acos", XaviFunction_acos);
	XaviFunctionCallerInstall("asin", XaviFunction_asin);
	XaviFunctionCallerInstall("atan", XaviFunction_atan);
	XaviFunctionCallerInstall("ceil", XaviFunction_ceil);
	XaviFunctionCallerInstall("cos", XaviFunction_cos);
	XaviFunctionCallerInstall("cosh", XaviFunction_cosh);
	XaviFunctionCallerInstall("exp", XaviFunction_exp);
	XaviFunctionCallerInstall("floor", XaviFunction_floor);
	XaviFunctionCallerInstall("log", XaviFunction_log);
	XaviFunctionCallerInstall("log10", XaviFunction_log10);
	XaviFunctionCallerInstall("sin", XaviFunction_sin);
	XaviFunctionCallerInstall("sinh", XaviFunction_sinh);
	XaviFunctionCallerInstall("sqrt", XaviFunction_sqrt);
	XaviFunctionCallerInstall("tan", XaviFunction_tan);
	XaviFunctionCallerInstall("tanh", XaviFunction_tanh);

	return -1;
memerr:
	XaviFunctionCallerTearDown();
	return 0;
}

void XaviFunctionCallerTearDown(void)
{
	int i;
	XaviFunctionChain * current;
	XaviFunctionChain * next;

	if (functionTable)
	{
		for (i = 0; i <= 255; i++)
		{
			if (functionTable[i])
			{
				current = functionTable[i];
				while (current)
				{
					next = current->next;
					free(current->id);
					free(current);
					current = next;
				}
			}
		}

		free(functionTable);
		functionTable = NULL;
	}
}

static XaviFunctionPointer GetFunction(const char *name)
{
	int index;
	XaviFunctionChain *current;

	index = XaviHash((const unsigned char *)name, strlen(name));
	current = functionTable[index];

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

XaviValue XaviFunctionCallerCall(const char *name, int argc, XaviValue *argv)
{
	XaviFunctionPointer f;
	XaviValue rVal;

	f = GetFunction(name);

	if (!f)
		rVal.Status = XAVI_BAD_FUNCTION;
	else
		rVal = f(argc, argv);

	return rVal;
}
