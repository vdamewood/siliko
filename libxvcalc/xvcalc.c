#include "xvcalc.h"
#include "tokenize.h"
#include "xvcalcix.h"

int xvcalc_yyparse(void);
void xvcalc_set_status(xvcalc_status new_status);
static xvcalc_status status = NONE;
static int    value_as_int   = 0;
static float  value_as_float = 0.0;
static char * error_message  = NULL;

xvcalc_status xvcalc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = xvcalc_yy_scan_string(inString);
	xvcalc_yy_switch_to_buffer(buffer);
	xvcalc_yyparse();
	return status;
}

int xvcalc_get_int(void)
{
	return value_as_int;
}

float xvcalc_get_float(void)
{
	return value_as_float;
}

char * xvcalc_error_message(void)
{
        return error_message;
}

void xvcalc_clean(void)
{
	xvcalc_set_status(NONE);
	value_as_int = 0;
	value_as_float = 0.0;
        if(error_message) {
		free(error_message);
		error_message = NULL;
	}
}

/* The following are internal-use only. */

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
}

void xvcalc_report_lex_error(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		xvcalc_set_malloc_error();
		return;
	}
	sprintf(message, t_message, bad_char);
	xvcalc_yyerror(message);
	free(message);
}

void xvcalc_yyerror(const char *s)
{
	if(!(error_message = malloc(strlen(s)+1))) {
		xvcalc_set_malloc_error();
		return;
	}
	
        strcpy(error_message, s);
	xvcalc_set_status(E_SYNTAX);
}
