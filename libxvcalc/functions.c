#include <string.h>
#include "functions.h"

number xvcalc_plg_abs(int count, number * arguments)
{
	number rVal;
	rVal = arguments[0];
	if (rVal.type == 'f' && rVal.f < 0.0)
		rVal.f = rVal.f * -1.0;
	else if (rVal.type == 'i' && rVal.i < 0)
		rVal.i = rVal.i * -1;
	return rVal;
}

function_ptr get_function(char * name)
{
	return xvcalc_plg_abs;
}