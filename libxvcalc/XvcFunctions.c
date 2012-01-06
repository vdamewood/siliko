#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "xvcalc.h" // For error codes
#include "XvcFunctions.h" // For xvcalc_call_function() declaration

static number xvcalc_func_abs(int argc, number * argv, jmp_buf jb)
{
	number rVal;

	if (argc != 1) longjmp(jb, E_ARGUMENTS);

	rVal = argv[0];
	if (rVal.type == 'f')
		rVal.f = fabs(rVal.f);
	else if (rVal.type == 'i')
		rVal.i = abs(rVal.i);
	return rVal;
}

static number xvcalc_func_sqrt(int argc, number * argv, jmp_buf jb)
{
	number rVal;
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
static number xvcalc_func_dummy(int argc, number * argv, jmp_buf jb)
{
	return argv[0];
}

typedef number (*function_ptr)(int, number *,jmp_buf);

static function_ptr xvcalc_get_function(const char * name)
{
	if (strcmp(name, "abs") == 0)
		return xvcalc_func_abs;
	else if (strcmp(name, "sqrt") == 0)
		return xvcalc_func_sqrt;
	else if (strcmp(name, "dummy") == 0)
		return xvcalc_func_dummy;
	else
		return NULL;
}

/****************************************\
|* Begin module's external interface.   *|
\****************************************/

number xvcalc_call_function(const char * name, int argc, number * argv, jmp_buf jb)
{
	function_ptr f;

	f = xvcalc_get_function(name);
	if (!f) longjmp(jb, E_FUNCTION);
	return f(argc, argv, jb);
}
