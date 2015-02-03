/*
 * Functions.c: Built-in functions
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

#include "Functions.h"

XaviValue XaviFunction_add(int argc, XaviValue *argv)
{
	XaviValue rVal;
	int i;

	if (!argc)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.status == XAVI_FLOAT)
		{
			if (argv[i].status == XAVI_FLOAT)
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
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.f = (float)rVal.i + argv[i].f;
				rVal.status = XAVI_FLOAT;
			}
			else
			{
				rVal.i += argv[i].i;
			}
		}
	}

	return rVal;
}

XaviValue XaviFunction_subtract(int argc, XaviValue *argv)
{
	XaviValue rVal;
	int i;

	if (!argc)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.status == XAVI_FLOAT)
		{
			if (argv[i].status == XAVI_FLOAT)
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
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.f = (float)rVal.i - argv[i].f;
				rVal.status = XAVI_FLOAT;
			}
			else
			{
				rVal.i -= argv[i].i;
			}
		}
	}

	return rVal;
}

XaviValue XaviFunction_multiply(int argc, XaviValue *argv)
{
	XaviValue rVal;
	int i;

	if (!argc)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.status == XAVI_FLOAT)
		{
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.f *= argv[i].f;
			}
			else
			{
				rVal.f *= (float)argv[i].i;
			}
		}
		else {
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.f = (float)rVal.i * argv[i].f;
				rVal.status = XAVI_FLOAT;
			}
			else
			{
				rVal.i *= argv[i].i;
			}
		}
	}

	return rVal;
}

XaviValue XaviFunction_divide(int argc, XaviValue *argv)
{
	XaviValue rVal;
	int i;

	if (argc < 2)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];

	for (i = 1; i < argc; i++)
	{
		/* Division-by-Zero Error */
		if ((argv[i].status == XAVI_FLOAT && argv[i].f == 0.0)
			|| (argv[i].status == XAVI_INTEGER && argv[i].i == 0))
		{
			rVal.status = XAVI_ZERO_DIV_ERR;
			return rVal;
		}

		if (rVal.status == XAVI_FLOAT)
		{
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.f /= argv[i].f;
			}
			else
			{
				rVal.f /= (float) argv[i].i;
			}
		}
		else {
			if (argv[i].status == XAVI_FLOAT)
			{
				rVal.status = XAVI_FLOAT;
				rVal.f = (float) rVal.i / argv[i].f;
			}
			else if (rVal.i % argv[i].i == 0)
			{
				rVal.i /= argv[i].i;
			}
			else
			{
				rVal.status = XAVI_FLOAT;
				rVal.f = (float) rVal.i / (float) argv[i].i;
			}
		}
	}
	return rVal;
}

XaviValue XaviFunction_power(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float runningValue;
	float nextValue;
	int i;

	runningValue = (argv[0].status == XAVI_INTEGER)
		? (float) argv[0].i
		: argv[0].f;

	for (i = 1; i < argc; i++)
	{
		nextValue = (argv[i].status == XAVI_INTEGER)
			? nextValue = (float) argv[i].i
			: argv[i].f;

		runningValue = (float) pow(runningValue, nextValue);
	}

	rVal.status = XAVI_FLOAT;
	rVal.f = runningValue;
	return rVal;
}


XaviValue XaviFunction_dice(int argc, XaviValue *argv)
{
	/* TODO: Make this function handle fractional dice. */
	static int hasSeeded = 0;
	int runningTotal = 0;
	XaviValue rVal;
	int count;
	int faces;
	int i;

	if(argc != 2)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	count = (argv[0].status == XAVI_INTEGER)
		? argv[0].i
		: (int) argv[0].f;

	faces = (argv[1].status == XAVI_INTEGER)
		? argv[1].i
		: (int) argv[1].f;

	if (!hasSeeded)
	{
		hasSeeded = 1;
		srand((unsigned int)time(NULL));
	}

	for (i = 1; i <= count; i++) runningTotal += (rand() % faces) + 1;
	rVal.status = XAVI_INTEGER;
	rVal.i = runningTotal;
	return rVal;
}

XaviValue XaviFunction_abs(int argc, XaviValue *argv)
{
	XaviValue rVal;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];
	if (rVal.status == XAVI_FLOAT)
		rVal.f = (float) fabs(rVal.f);
	else if (rVal.status == XAVI_INTEGER)
		rVal.i = abs(rVal.i);
	return rVal;
}

XaviValue XaviFunction_acos(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	if (input < -1 || input > 1)
	{
		rVal.status = XAVI_DOMAIN_ERR;
		return rVal;
	}

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) acos(input);
	return rVal;
}

XaviValue XaviFunction_asin(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	if (input < -1 || input > 1)
	{
		rVal.status = XAVI_DOMAIN_ERR;
		return rVal;
	}

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) asin(input);
	return rVal;
}

XaviValue XaviFunction_atan(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float) argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) atan(input);
	return rVal;
}

XaviValue XaviFunction_ceil(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	result = (float) ceil(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = (int)result;
	}
	else
	{
		rVal.status = XAVI_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

XaviValue XaviFunction_cos(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) cos(input);
	return rVal;
}

XaviValue XaviFunction_cosh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) cosh(input);
	return rVal;
}

XaviValue XaviFunction_exp(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) exp(input);
	return rVal;
}

XaviValue XaviFunction_floor(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	result = (float) floor(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = (int)result;
	}
	else
	{
		rVal.status = XAVI_FLOAT;
		rVal.f = result;
	}
	return rVal;
}

XaviValue XaviFunction_log(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) log(input);
	return rVal;
}

XaviValue XaviFunction_log10(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) log10(input);
	return rVal;
}

XaviValue XaviFunction_sin(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) sin(input);
	return rVal;
}

XaviValue XaviFunction_sinh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) sinh(input);
	return rVal;
}

XaviValue XaviFunction_sqrt(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float inVal;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if ((argv[0].status == XAVI_INTEGER && argv[0].i < 0)
		|| (argv[0].status == XAVI_FLOAT && argv[0].f < 0.0))
	{
			rVal.status = XAVI_DOMAIN_ERR;
			return rVal;
	}

	if (argv[0].status == XAVI_FLOAT)
	{
		inVal = argv[0].f;
	}
	else
	{
		inVal = (float) argv[0].i;
	}

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) sqrt(inVal);

	return rVal;
}

XaviValue XaviFunction_tan(int argc, XaviValue *argv)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER) input = (float)argv[0].i;
	else input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) tan(input);
	return rVal;
}

XaviValue XaviFunction_tanh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].status == XAVI_INTEGER)
		input = (float)argv[0].i;
	else
		input = argv[0].f;

	rVal.status = XAVI_FLOAT;
	rVal.f = (float) tanh(input);
	return rVal;
}
