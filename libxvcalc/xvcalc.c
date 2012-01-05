#include "xvcalc.h"
#include "tokenize.h"
#include "XvcState.h"

int xvcalc_yyparse(void);

void xvcalc_open(void)
{
}

void xvcalc_close(void)
{
	xvcalc_clean();
}

xvcalc_status xvcalc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = xvcalc_yy_scan_string(inString);
	xvcalc_yy_switch_to_buffer(buffer);
	xvcalc_yyparse();
	return XvcStateStatus();
}

int xvcalc_get_int(void)
{
	return XvcStateInteger();
}

float xvcalc_get_float(void)
{
	return XvcStateFloat();
}

const char * xvcalc_error_message(void)
{
        return XvcStateErrorMessage();
}

void xvcalc_clean(void)
{
	XvcStateSetStatus(NONE);
	XvcStateSetInteger(0);
	XvcStateSetFloat(0.0);
	XvcStateSetErrorMessage(NULL);
}
