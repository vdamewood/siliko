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
