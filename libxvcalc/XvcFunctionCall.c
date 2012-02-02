/*
 * XvcFunctionCall.c: Function handling.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "XVCalc.h"
#include "XvcFunctionCall.h"

static XvcNumber XvcFunction_abs(int argc, XvcNumber * argv)
{
	XvcNumber rVal;

	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];
	if (rVal.status == S_FLOAT)
		rVal.f = fabs(rVal.f);
	else if (rVal.status == S_INTEGER)
		rVal.i = abs(rVal.i);
	return rVal;
}

static XvcNumber XvcFunction_sqrt(int argc, XvcNumber * argv)
{
	XvcNumber rVal;
	float inVal;

	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (
		(argv[0].status == S_INTEGER && argv[0].i < 0)
		|| (argv[0].status == S_FLOAT && argv[0].f < 0.0)) {
			rVal.status = E_DOMAIN;
			return rVal;
	}
	
	if (argv[0].status == S_FLOAT) {
		inVal = argv[0].f;
	}
	else {
		inVal = (float) argv[0].i;
	}
	rVal.status = S_FLOAT;
	rVal.f = sqrt(inVal);
	
	return rVal;
}

// This will be removed when more functions are implemented.
static XvcNumber XvcFunction_dummy(int argc, XvcNumber * argv)
{
	return argv[0];
}

////////////////////////////////////////////////////////////////////////

static void ShiftBits(unsigned char * input, int len)
{
	int i;
	unsigned char tmp;
	
	for(i = len-1; i >= 0; i--)
	{
		input[i] >>= 1;
		if (i) {
			tmp = input[i-1] & 1;
			tmp <<= 7;
			input[i] |= tmp;
		}
	}
}

#define EXP1 0xEA
#define EXP2 0x80

static unsigned char XvcCrc8(const unsigned char * input)
{
	int len;
	unsigned char * divisor;
	unsigned char * result;
	unsigned char rVal;
	unsigned char mask;
	
	int i;
	int j;
	
	// The terminating '\0' as part of the CRC.
	// We can do this because we won't be printing the
	// series of characters.
	len = strlen(input) + 1;
	divisor = malloc(len);
	memset(divisor, 0, len);
	result = malloc(len);
	strcpy(result, input);
	divisor[0] = EXP1;
	divisor[1] = EXP2;
	
	for (i = 0; i < len-1; i++) {
		mask = 0x80;
		for (j = 0; j <= 7; j++) {
			if (mask & result[i]) {
				result[i] ^= divisor[i];
				result[i+1] ^= divisor[i+1];
			}
			ShiftBits(divisor, len);
			mask >>= 1;
		}
	}
	
	rVal = result[len-1];
	free(result);
	free(divisor);
	return rVal;
}


////////////////////////////////////////////////////////////////////////

typedef XvcNumber (*FunctionPointer)(int, XvcNumber *);

#define FUNCTION_MAX 2
static char *functionNames[] = {"abs", "sqrt", "dummy"};
static FunctionPointer functions[] = {
	XvcFunction_abs,
	XvcFunction_sqrt,
	XvcFunction_dummy};

struct XvcFunctionChain
{
	char * id;
	FunctionPointer function;
	struct XvcFunctionChain * next;
};
typedef struct XvcFunctionChain XvcFunctionChain;

static XvcFunctionChain ** functionTable;

int XvcFunctionCallOpen()
{
	int i;
	int index;
	XvcFunctionChain * currentChain;
	functionTable = malloc(256 * sizeof(XvcFunctionChain));
	for (i = 0; i<=255; i++) {
		functionTable[i] = NULL;
	}
	

	for (i = 0; i <= FUNCTION_MAX; i++) {
		index = XvcCrc8(functionNames[i]);
		if (functionTable[index]) {
			currentChain = functionTable[index];
			while (currentChain->next) {
				currentChain = currentChain->next;
			}
			currentChain->next = malloc(sizeof (XvcFunctionChain));
			currentChain = currentChain->next;
		}
		else {
			functionTable[index] = malloc(sizeof (XvcFunctionChain));
			currentChain = functionTable[index];
		}

		currentChain->id = functionNames[i];
		currentChain->function = functions[i];
		currentChain->next = NULL;
	}
	return 1;
}

void XvcFunctionCallClose()
{
	int i;
	XvcFunctionChain * current;
	XvcFunctionChain * next;
	
	for (i = 0; i <= 255; i++)
	{
		if (functionTable[i]) {
			current = functionTable[i];
			while (current) {
				next = current->next;
				free(current);
				current = next;
			}
		}
	}
	free(functionTable);
	functionTable = NULL;
}

static FunctionPointer GetFunction(const char * name)
{
	int idx;
	XvcFunctionChain * currentChain;

	idx = XvcCrc8(name);

	currentChain = functionTable[idx];
	while (currentChain) {
		if (strcmp(name, currentChain->id) != 0) {
			currentChain = currentChain->next;
		}
		else {
			break;
		}
	}

	if (!functionTable[idx]) {
		return NULL;
	}
	else {
		return currentChain->function;
	}	
}

XvcNumber XvcFunctionCall(const char * name, int argc, XvcNumber * argv)
{
	FunctionPointer f;
	XvcNumber rVal;
	
	f = GetFunction(name);
	if (!f) {
		rVal.status = E_FUNCTION;
	}
	else {
		rVal = f(argc, argv);
	}
	return rVal;
}
