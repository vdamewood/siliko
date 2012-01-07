#include <stdlib.h>
#include <string.h>

#include "XvcFunctionId.h"
#include "XvcStructs.h"
#include "XvcCleanup.h"



char * XvcFunctionIdNew(char * in_token)
{
	char * rVal;
	rVal = malloc(strlen(in_token)+1);
	xvcalc_cache_dangling_id(rVal);

	strcpy(rVal, in_token);
	return rVal;
}

void XvcFunctionIdDelete(char * in_token)
{
	xvcalc_release_dangling_id(in_token);
	free(in_token);
}
