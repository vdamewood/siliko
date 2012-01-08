#include "XVCalc.h"
#include "XvcState.h"
#include "XvcYyLexer.h"

int xvcalc_yyparse(void);

void XvcOpen(void)
{
}

void XvcClose(void)
{
	XvcStateClear();
}

XvcStatus XvcParse(char *inString)
{
	XvcStateClear();
	YY_BUFFER_STATE buffer;
	buffer = xvcalc_yy_scan_string(inString);
	xvcalc_yy_switch_to_buffer(buffer);
	xvcalc_yyparse();
	return XvcStateStatus();
}

int XvcGetInteger(void)
{
	return XvcStateInteger();
}

float XvcGetFloat(void)
{
	return XvcStateFloat();
}
