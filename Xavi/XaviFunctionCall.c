/*
 * XaviFunctionCall.c: Function handling.
 * Copyright 2012, 2014 Vincent Damewood
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

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Xavi.h"
#include "XaviFunctionCall.h"

static XaviNumber XaviFunction_add(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	int i;

	if (!argc)
	{
		rVal.status = S_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{	
		if (rVal.status == S_FLOAT)
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f += argv[i].f;
			}
			else
			{
				rVal.f += (float)argv[i].i;
			}
		}
		else
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f = (float)rVal.i + argv[i].f;
				rVal.status = S_FLOAT;
			}
			else
			{
				rVal.i += argv[i].i;
			}
		}
	}

	return rVal;
}

static XaviNumber XaviFunction_subtract(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	int i;

	if (!argc)
	{
		rVal.status = S_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{	
		if (rVal.status == S_FLOAT)
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f -= argv[i].f;
			}
			else
			{
				rVal.f -= (float)argv[i].i;
			}
		}
		else
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f = (float)rVal.i - argv[i].f;
				rVal.status = S_FLOAT;
			}
			else
			{
				rVal.i -= argv[i].i;
			}
		}
	}

	return rVal;
}

static XaviNumber XaviFunction_multiply(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	int i;

	if (!argc)
	{
		rVal.status = S_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{	
		if (rVal.status == S_FLOAT)
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f *= argv[i].f;
			}
			else
			{
				rVal.f *= (float)argv[i].i;
			}
		}
		else {
			if (argv[i].status == S_FLOAT)
			{
				rVal.f = (float)rVal.i * argv[i].f;
				rVal.status = S_FLOAT;
			}
			else
			{
				rVal.i *= argv[i].i;
			}
		}
	}

	return rVal;
}

static XaviNumber XaviFunction_divide(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	int i;

	if (argc < 2)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];

	for (i = 1; i < argc; i++)
	{
		/* Division-by-Zero Error */
		if ((argv[i].status == S_FLOAT && argv[i].f == 0.0)
			|| (argv[i].status == S_INTEGER && argv[i].i == 0))
		{
			rVal.status = E_ZERO_DIV;
			return rVal;
		}
		
		if (rVal.status == S_FLOAT)
		{
			if (argv[i].status == S_FLOAT)
			{
				rVal.f /= argv[i].f;
			}
			else
			{
				rVal.f /= (float) argv[i].i;
			}
		}
		else {
			if (argv[i].status == S_FLOAT)
			{
				rVal.f = (float) rVal.i / argv[i].f;
			}
			else if (rVal.i % argv[i].i == 0)
			{
				rVal.i /= argv[i].i;
			}
			else
			{
				rVal.status = S_FLOAT;
				rVal.f = (float) rVal.i / (float) argv[i].i;
			}
		}
	}
	return rVal;
}

static XaviNumber XaviFunction_power(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float runningValue;
	float nextValue;
	int i;

	runningValue = (argv[0].status == S_INTEGER)
		? (float) argv[0].i
		: argv[0].f;

	for (i = 1; i < argc; i++)
	{
		nextValue = (argv[i].status == S_INTEGER)
			? nextValue = (float) argv[i].i
			: argv[i].f;

		runningValue = pow(runningValue, nextValue);
	}

	rVal.status = S_FLOAT;
	rVal.f = runningValue;
	return rVal;
}


static XaviNumber XaviFunction_dice(int argc, XaviNumber *argv)
{
	/* TODO: Make this function handle fractional dice. */
	static int hasSeeded = 0;
	int runningTotal = 0;
	XaviNumber rVal;
	int count;
	int faces;
	int i;
	
	if(argc != 2)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	count = (argv[0].status == S_INTEGER)
		? argv[0].i
		: (int) argv[0].f;
	
	faces = (argv[1].status == S_INTEGER)
		? argv[1].i
		: (int) argv[1].f;
		
	if (!hasSeeded)
	{
		hasSeeded = 1;
		srand((unsigned int)time(NULL));
	}
	
	for (i = 1; i <= count; i++) runningTotal += (rand() % faces) + 1;
	rVal.status = S_INTEGER;
	rVal.i = runningTotal;
	return rVal;
}

static XaviNumber XaviFunction_abs(int argc, XaviNumber *argv)
{
	XaviNumber rVal;

	if (argc != 1)
	{
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

static XaviNumber XaviFunction_acos(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	if (input < -1 || input > 1)
	{
		rVal.status = E_DOMAIN;
		return rVal;
	}

	rVal.status = S_FLOAT;
	rVal.f = acos(input);
	return rVal;
}

static XaviNumber XaviFunction_asin(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	if (input < -1 || input > 1)
	{
		rVal.status = E_DOMAIN;
		return rVal;
	}

	rVal.status = S_FLOAT;
	rVal.f = asin(input);
	return rVal;
}

static XaviNumber XaviFunction_atan(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = atan(input);
	return rVal;
}

static XaviNumber XaviFunction_ceil(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	result = ceil(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.status = S_INTEGER;
		rVal.i = (int)result;
	}
	else
	{
		rVal.status = S_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

static XaviNumber XaviFunction_cos(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = cos(input);
	return rVal;
}

static XaviNumber XaviFunction_cosh(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = cosh(input);
	return rVal;
}

static XaviNumber XaviFunction_exp(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = exp(input);
	return rVal;
}

static XaviNumber XaviFunction_floor(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	result = floor(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.status = S_INTEGER;
		rVal.i = (int)result;
	}
	else
	{
		rVal.status = S_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

static XaviNumber XaviFunction_log(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = log(input);
	return rVal;
}

static XaviNumber XaviFunction_log10(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = log10(input);
	return rVal;
}

static XaviNumber XaviFunction_sin(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = sin(input);
	return rVal;
}

static XaviNumber XaviFunction_sinh(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = sinh(input);
	return rVal;
}

static XaviNumber XaviFunction_sqrt(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float inVal;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if ((argv[0].status == S_INTEGER && argv[0].i < 0)
		|| (argv[0].status == S_FLOAT && argv[0].f < 0.0))
	{
			rVal.status = E_DOMAIN;
			return rVal;
	}
	
	if (argv[0].status == S_FLOAT)
	{
		inVal = argv[0].f;
	}
	else
	{
		inVal = (float) argv[0].i;
	}

	rVal.status = S_FLOAT;
	rVal.f = sqrt(inVal);

	return rVal;
}

static XaviNumber XaviFunction_tan(int argc, XaviNumber *argv)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER) input = (float)argv[0].i;
	else input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = tan(input);
	return rVal;
}

static XaviNumber XaviFunction_tanh(int argc, XaviNumber *argv)
{
	XaviNumber rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = E_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == S_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = S_FLOAT;
	rVal.f = tanh(input);
	return rVal;
}

////////////////////////////////////////////////////////////////////////

static void ShiftBits(unsigned char * input, int len)
{
	int i;
	unsigned char tmp;

	for(i = len-1; i >= 0; i--)
	{
		input[i] >>= 1;
		if (i)
		{
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
	// FIXME: This function malloc()s a chunk of memory the same size as the
	// input, when a small arrays on the stack will suffice.
	int len;
	unsigned char * divisor;
	unsigned char * result;
	unsigned char rVal;
	unsigned char mask;

	int i;
	int j;

	// We use the terminating '\0' as part of the CRC.
	// We can do this because we won't be printing the
	// series of characters.
	len = strlen(input) + 1;
	divisor = malloc(len);
	memset(divisor, 0, len);
	result = malloc(len);
	strcpy(result, input);
	divisor[0] = EXP1;
	divisor[1] = EXP2;

	for (i = 0; i < len-1; i++)
	{
		mask = 0x80;
		for (j = 0; j <= 7; j++)
		{
			if (mask & result[i])
			{
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

#define FUNCTION_MAX 21
static char *functionNames[] =
{
	"add", "subtract", "multiply", "divide",
	"power", "dice",
	"abs", "agit cos", "asin", "atan",
	"ceil", "cos", "cosh", "exp",
	"floor", "log","log10", "sin",
	 "sinh", "sqrt", "tan", "tanh"
};

static FunctionPointer functions[] =
{
	XaviFunction_add, XaviFunction_subtract, XaviFunction_multiply, XaviFunction_divide,
	XaviFunction_power, XaviFunction_dice,
	XaviFunction_abs, XaviFunction_acos, XaviFunction_asin, XaviFunction_atan,
	XaviFunction_ceil, XaviFunction_cos, XaviFunction_cosh, XaviFunction_exp,
	XaviFunction_floor, XaviFunction_log, XaviFunction_log10, XaviFunction_sin,
	XaviFunction_sinh, XaviFunction_sqrt, XaviFunction_tan, XaviFunction_tanh
};

struct XaviFunctionChain
{
	char *id;
	FunctionPointer function;
	struct XaviFunctionChain *next;
};
typedef struct XaviFunctionChain XaviFunctionChain;

static XaviFunctionChain **functionTable;

int XaviFunctionCallOpen()
{
	// FIXME: This function makes calls to malloc() without checking the return
	// value. Rewrite it to either succeed or fail completely, instead.
	int i;
	int index;
	XaviFunctionChain *currentChain;
	if (!(functionTable = malloc(256 * sizeof(XaviFunctionChain))))
		return 0;

	for (i = 0; i<=255; i++)
		functionTable[i] = NULL;

	for (i = 0; i <= FUNCTION_MAX; i++)
	{
		index = XaviCrc8(functionNames[i]);
		if (functionTable[index])
		{
			currentChain = functionTable[index];
			while (currentChain->next)
				currentChain = currentChain->next;
			currentChain->next = malloc(sizeof(XaviFunctionChain));
			currentChain = currentChain->next;
		}
		else
		{
			functionTable[index] = malloc(sizeof(XaviFunctionChain));
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
		if (functionTable[i])
		{
			current = functionTable[i];
			while (current)
			{
				next = current->next;
				free(current);
				current = next;
			}
		}
	}

	free(functionTable);
	functionTable = NULL;
}

static FunctionPointer GetFunction(const char *name)
{
	int index;
	XaviFunctionChain *current;

	index = XaviCrc8(name);

	current = functionTable[index];
	while (current)
		if (strcmp(name, current->id) != 0)
			current = current->next;
		else
			break;

	if (current)
		return current->function;

	return NULL;
}

XaviNumber XaviFunctionCall(const char *name, int argc, XaviNumber *argv)
{
	FunctionPointer f;
	XaviNumber rVal;

	f = GetFunction(name);

	if (!f)
		rVal.status = E_FUNCTION;
	else
		rVal = f(argc, argv);

	return rVal;
}
