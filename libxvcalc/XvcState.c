#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "XvcState.h"

static xvcalc_status status = NONE;
static int    value_as_int   = 0;
static float  value_as_float = 0.0;
static char * error_message  = NULL;

xvcalc_status XvcStateStatus()
{
	return status;
}

void XvcStateSetStatus(xvcalc_status NewStatus)
{
	status = NewStatus;
}

int XvcStateInteger()
{
	return value_as_int;
}

void XvcStateSetInteger(int NewInteger)
{
	value_as_int = NewInteger;
}

float XvcStateFloat()
{
	return value_as_float;
}

void XvcStateSetFloat(float NewFloat)
{
	value_as_float = NewFloat;
}

const char * XvcStateErrorMessage()
{
	return error_message;
}

void XvcStateSetErrorMessage(const char * NewErrorMessage)
{
	free(error_message);

	if (NewErrorMessage) {
		if(!(error_message = malloc(strlen(NewErrorMessage)+1))) {
			xvcalc_set_malloc_error();
			return;
		}
		strcpy(error_message, NewErrorMessage);
	}
	else {
		error_message = NULL;
	}
}

void xvcalc_set_status(xvcalc_status new_status)
{
	status = new_status;
}

void xvcalc_set_nil()
{
	xvcalc_set_status(S_INTEGER);
	value_as_int = 0;
	value_as_float = 0.0;
}

void xvcalc_set_value(number new_value)
{
	switch (new_value.type) {
		case 'i':
			xvcalc_set_status(S_INTEGER);
			value_as_int = new_value.i;
			value_as_float = (float) new_value.i;
			break;
		case 'f':
			xvcalc_set_status(S_FLOAT);
			value_as_int = (int) new_value.f;
			value_as_float = new_value.f;
	};
}

void xvcalc_set_malloc_error(void)
{
	xvcalc_set_status(E_MEMORY);
	free(error_message);
	error_message = NULL;
}
