/*
 * XaviFunctionCall.c: Function handling.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "Xavi.h"
#include "XaviFunctionCall.h"

static XaviNumber XaviFunction_abs(int argc, XaviNumber * argv)
{
	XaviNumber rVal;

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

static XaviNumber XaviFunction_acos(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_asin(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_atan(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_atan2(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_ceil(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	float input;
	float result;
	
	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (argv[0].status == S_INTEGER) input = (float) argv[0].i;
	else input = argv[0].f;
	
	result = ceil(input);
	if (result <= INT_MAX && result >= INT_MIN) {
		rVal.status = S_INTEGER;
		rVal.i = (int) result;
	}
	else {
		rVal.status = S_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

static XaviNumber XaviFunction_cos(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	float input;
	
	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (argv[0].status == S_INTEGER) input = (float) argv[0].i;
	else input = argv[0].f;
	
	rVal.status = S_FLOAT;
	rVal.f = cos(input);
	return rVal;
}

static XaviNumber XaviFunction_cosh(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_exp(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_floor(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	float input;
	float result;

	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (argv[0].status == S_INTEGER) input = (float) argv[0].i;
	else input = argv[0].f;

	result = floor(input);
	if (result <= INT_MAX && result >= INT_MIN) {
		rVal.status = S_INTEGER;
		rVal.i = (int) result;
	}
	else {
		rVal.status = S_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

static XaviNumber XaviFunction_ldexp(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_log(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_log10(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_sin(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	float input;
	
	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (argv[0].status == S_INTEGER) input = (float) argv[0].i;
	else input = argv[0].f;
	
	rVal.status = S_FLOAT;
	rVal.f = sin(input);
	return rVal;
}

static XaviNumber XaviFunction_sinh(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

static XaviNumber XaviFunction_sqrt(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
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

static XaviNumber XaviFunction_tan(int argc, XaviNumber * argv)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	XaviNumber rVal;
	float input;
	
	if (argc != 1) {
		rVal.status = E_ARGUMENTS;
		return rVal;
	}
	
	if (argv[0].status == S_INTEGER) input = (float) argv[0].i;
	else input = argv[0].f;
	
	rVal.status = S_FLOAT;
	rVal.f = tan(input);
	return rVal;
}

static XaviNumber XaviFunction_tanh(int argc, XaviNumber * argv)
{
	XaviNumber rVal;
	rVal.status = E_FUNCTION;
	return rVal;
}

// This will be removed when more functions are implemented.
static XaviNumber XaviFunction_dummy(int argc, XaviNumber * argv)
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

static unsigned char XaviCrc8(const unsigned char * input)
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

typedef XaviNumber (*FunctionPointer)(int, XaviNumber *);

#define FUNCTION_MAX 18
static char *functionNames[] = {
	"abs", "acos", "asin", "atan",
	"atan2", "ceil", "cos", "cosh",
	"exp", "floor", "ldexp", "log",
	"log10", "sin", "sinh",	 "sqrt",
	"tan", "tanh", "dummy"};
static FunctionPointer functions[] = {
	XaviFunction_abs, XaviFunction_acos, XaviFunction_asin, XaviFunction_atan,
	XaviFunction_atan2, XaviFunction_ceil, XaviFunction_cos, XaviFunction_cosh,
	XaviFunction_exp, XaviFunction_floor, XaviFunction_ldexp, XaviFunction_log,
	XaviFunction_log10, XaviFunction_sin, XaviFunction_sinh, XaviFunction_sqrt,
	XaviFunction_tan, XaviFunction_tanh, XaviFunction_dummy};

struct XaviFunctionChain
{
	char * id;
	FunctionPointer function;
	struct XaviFunctionChain * next;
};
typedef struct XaviFunctionChain XaviFunctionChain;

static XaviFunctionChain ** functionTable;

int XaviFunctionCallOpen()
{
	int i;
	int index;
	XaviFunctionChain * currentChain;
	functionTable = malloc(256 * sizeof(XaviFunctionChain));
	for (i = 0; i<=255; i++) {
		functionTable[i] = NULL;
	}
	

	for (i = 0; i <= FUNCTION_MAX; i++) {
		index = XaviCrc8(functionNames[i]);
		if (functionTable[index]) {
			currentChain = functionTable[index];
			while (currentChain->next) {
				currentChain = currentChain->next;
			}
			currentChain->next = malloc(sizeof (XaviFunctionChain));
			currentChain = currentChain->next;
		}
		else {
			functionTable[index] = malloc(sizeof (XaviFunctionChain));
			currentChain = functionTable[index];
		}

		currentChain->id = functionNames[i];
		currentChain->function = functions[i];
		currentChain->next = NULL;
	}
	return 1;
}

void XaviFunctionCallClose()
{
	int i;
	XaviFunctionChain * current;
	XaviFunctionChain * next;
	
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
	int index;
	XaviFunctionChain * current;

	index = XaviCrc8(name);

	current = functionTable[index];
	while (current) {
		if (strcmp(name, current->id) != 0) {
			current = current->next;
		}
		else {
			break;
		}
	}

	if (current) {
		return current->function;
	}
	return NULL;
}

XaviNumber XaviFunctionCall(const char * name, int argc, XaviNumber * argv)
{
	FunctionPointer f;
	XaviNumber rVal;
	
	f = GetFunction(name);
	if (!f) {
		rVal.status = E_FUNCTION;
	}
	else {
		rVal = f(argc, argv);
	}
	return rVal;
}
