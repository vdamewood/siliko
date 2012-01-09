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

static XvcNumber XvcFunction_abs(int argc, XvcNumber * argv, jmp_buf jb)
{
	XvcNumber rVal;

	if (argc != 1) longjmp(jb, E_ARGUMENTS);

	rVal = argv[0];
	if (rVal.type == 'f')
		rVal.f = fabs(rVal.f);
	else if (rVal.type == 'i')
		rVal.i = abs(rVal.i);
	return rVal;
}

static XvcNumber XvcFunction_sqrt(int argc, XvcNumber * argv, jmp_buf jb)
{
	XvcNumber rVal;
	float inVal;
	if (argc != 1) longjmp(jb, E_ARGUMENTS);
	
	if (
		(argv[0].type == 'i' && argv[0].i < 0)
		|| (argv[0].type == 'f' && argv[0].f < 0.0)) {
		longjmp(jb, E_DOMAIN);
	}
	
	if (argv[0].type == 'f') {
		inVal = argv[0].f;
	}
	else {
		inVal = (float) argv[0].i;
	}
	rVal.type = 'f';
	rVal.f = sqrt(inVal);
	
	return rVal;
}

// This will be removed when more functions are implemented.
static XvcNumber XvcFunction_dummy(int argc, XvcNumber * argv, jmp_buf jb)
{
	return argv[0];
}

typedef XvcNumber (*FunctionPointer)(int, XvcNumber *,jmp_buf);

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

XvcNumber XvcFunctionCall(const char * name, int argc, XvcNumber * argv, jmp_buf jb)
{
	FunctionPointer f;

	f = GetFunction(name);
	if (!f) longjmp(jb, E_FUNCTION);
	return f(argc, argv, jb);
}
