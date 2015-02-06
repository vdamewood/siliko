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
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == XAVI_FLOAT)
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float += argv[i].Float;
			}
			else
			{
				rVal.Float += (float)argv[i].Integer;
			}
		}
		else
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float = (float)rVal.Integer + argv[i].Float;
				rVal.Status = XAVI_FLOAT;
			}
			else
			{
				rVal.Integer += argv[i].Integer;
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
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == XAVI_FLOAT)
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float -= argv[i].Float;
			}
			else
			{
				rVal.Float -= (float)argv[i].Integer;
			}
		}
		else
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float = (float)rVal.Integer - argv[i].Float;
				rVal.Status = XAVI_FLOAT;
			}
			else
			{
				rVal.Integer -= argv[i].Integer;
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
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = 0;
		return rVal;
	}

	rVal = argv[0];

	for(i = 1; i < argc; i++)
	{
		if (rVal.Status == XAVI_FLOAT)
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float *= argv[i].Float;
			}
			else
			{
				rVal.Float *= (float)argv[i].Integer;
			}
		}
		else {
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float = (float)rVal.Integer * argv[i].Float;
				rVal.Status = XAVI_FLOAT;
			}
			else
			{
				rVal.Integer *= argv[i].Integer;
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
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];

	for (i = 1; i < argc; i++)
	{
		/* Division-by-Zero Error */
		if ((argv[i].Status == XAVI_FLOAT && argv[i].Float == 0.0)
			|| (argv[i].Status == XAVI_INTEGER && argv[i].Integer == 0))
		{
			rVal.Status = XAVI_ZERO_DIV_ERR;
			return rVal;
		}

		if (rVal.Status == XAVI_FLOAT)
		{
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Float /= argv[i].Float;
			}
			else
			{
				rVal.Float /= (float) argv[i].Integer;
			}
		}
		else {
			if (argv[i].Status == XAVI_FLOAT)
			{
				rVal.Status = XAVI_FLOAT;
				rVal.Float = (float) rVal.Integer / argv[i].Float;
			}
			else if (rVal.Integer % argv[i].Integer == 0)
			{
				rVal.Integer /= argv[i].Integer;
			}
			else
			{
				rVal.Status = XAVI_FLOAT;
				rVal.Float = (float) rVal.Integer / (float) argv[i].Integer;
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

	runningValue = (argv[0].Status == XAVI_INTEGER)
		? (float) argv[0].Integer
		: argv[0].Float;

	for (i = 1; i < argc; i++)
	{
		nextValue = (argv[i].Status == XAVI_INTEGER)
			? nextValue = (float) argv[i].Integer
			: argv[i].Float;

		runningValue = (float) pow(runningValue, nextValue);
	}

	rVal.Status = XAVI_FLOAT;
	rVal.Float = runningValue;
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
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	count = (argv[0].Status == XAVI_INTEGER)
		? argv[0].Integer
		: (int) argv[0].Float;

	faces = (argv[1].Status == XAVI_INTEGER)
		? argv[1].Integer
		: (int) argv[1].Float;

	if (!hasSeeded)
	{
		hasSeeded = 1;
		srand((unsigned int)time(NULL));
	}

	for (i = 1; i <= count; i++) runningTotal += (rand() % faces) + 1;
	rVal.Status = XAVI_INTEGER;
	rVal.Integer = runningTotal;
	return rVal;
}

XaviValue XaviFunction_abs(int argc, XaviValue *argv)
{
	XaviValue rVal;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	rVal = argv[0];
	if (rVal.Status == XAVI_FLOAT)
		rVal.Float = (float) fabs(rVal.Float);
	else if (rVal.Status == XAVI_INTEGER)
		rVal.Integer = abs(rVal.Integer);
	return rVal;
}

XaviValue XaviFunction_acos(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float) argv[0].Integer;
	else
		input = argv[0].Float;

	if (input < -1 || input > 1)
	{
		rVal.Status = XAVI_DOMAIN_ERR;
		return rVal;
	}

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) acos(input);
	return rVal;
}

XaviValue XaviFunction_asin(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float) argv[0].Integer;
	else
		input = argv[0].Float;

	if (input < -1 || input > 1)
	{
		rVal.Status = XAVI_DOMAIN_ERR;
		return rVal;
	}

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) asin(input);
	return rVal;
}

XaviValue XaviFunction_atan(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float) argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) atan(input);
	return rVal;
}

XaviValue XaviFunction_ceil(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	result = (float) ceil(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = (int)result;
	}
	else
	{
		rVal.Status = XAVI_FLOAT;
		rVal.Float = result;
	}
	return rVal;
}

XaviValue XaviFunction_cos(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) cos(input);
	return rVal;
}

XaviValue XaviFunction_cosh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) cosh(input);
	return rVal;
}

XaviValue XaviFunction_exp(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) exp(input);
	return rVal;
}

XaviValue XaviFunction_floor(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;
	float result;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	result = (float) floor(input);
	if (result <= INT_MAX && result >= INT_MIN)
	{
		rVal.Status = XAVI_INTEGER;
		rVal.Integer = (int)result;
	}
	else
	{
		rVal.Status = XAVI_FLOAT;
		rVal.Float = result;
	}
	return rVal;
}

XaviValue XaviFunction_log(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) log(input);
	return rVal;
}

XaviValue XaviFunction_log10(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) log10(input);
	return rVal;
}

XaviValue XaviFunction_sin(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) sin(input);
	return rVal;
}

XaviValue XaviFunction_sinh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) sinh(input);
	return rVal;
}

XaviValue XaviFunction_sqrt(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float inVal;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if ((argv[0].Status == XAVI_INTEGER && argv[0].Integer < 0)
		|| (argv[0].Status == XAVI_FLOAT && argv[0].Float < 0.0))
	{
			rVal.Status = XAVI_DOMAIN_ERR;
			return rVal;
	}

	if (argv[0].Status == XAVI_FLOAT)
	{
		inVal = argv[0].Float;
	}
	else
	{
		inVal = (float) argv[0].Integer;
	}

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) sqrt(inVal);

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
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER) input = (float)argv[0].Integer;
	else input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) tan(input);
	return rVal;
}

XaviValue XaviFunction_tanh(int argc, XaviValue *argv)
{
	XaviValue rVal;
	float input;

	if (argc != 1)
	{
		rVal.Status = XAVI_BAD_ARGUMENTS;
		return rVal;
	}

	if (argv[0].Status == XAVI_INTEGER)
		input = (float)argv[0].Integer;
	else
		input = argv[0].Float;

	rVal.Status = XAVI_FLOAT;
	rVal.Float = (float) tanh(input);
	return rVal;
}
