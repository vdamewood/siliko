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

typedef XvcNumber (*FunctionPointer)(int, XvcNumber *);

static FunctionPointer GetFunction(const char * name)
{
	if (strcmp(name, "abs") == 0)
		return XvcFunction_abs;
	else if (strcmp(name, "sqrt") == 0)
		return XvcFunction_sqrt;
	else if (strcmp(name, "dummy") == 0)
		return XvcFunction_dummy;
	else
		return NULL;
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
