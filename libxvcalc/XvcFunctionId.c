#include <stdlib.h>
#include <string.h>

#include "XvcFunctionId.h"
#include "XvcStructs.h"
#include "XvcCleanup.h"


char * XvcFunctionIdNew(char * NewId)
{
	char * rVal;
	rVal = malloc(strlen(NewId)+1);
	XvcCleanupCacheFunctionId(rVal);
	strcpy(rVal, NewId);
	return rVal;
}

void XvcFunctionIdDelete(char * InId)
{
	XvcCleanupReleaseFunctionId(InId);
	free(InId);
}
