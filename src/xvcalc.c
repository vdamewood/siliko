#include "xvcalc.h"
#include "tokenize.h"
#include "xvcalcix.h"

int xvcalc_yyparse(void);

static char   status         = '\0';
static int    value_as_int   = 0;
static float  value_as_float = 0.0;
static char * error_message  = NULL;

char xvcalc(char *inString)
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

char *xvcalc_error_message(void)
{
        return error_message;
}

void xvcalc_clean(void)
{
	status = '\0';
	value_as_int = 0;
	value_as_float = 0.0;
        if(error_message) {
		free(error_message);
		error_message = NULL;
	}
}

/* The following are internal-use only. */

void xvcalc_set_nil()
{
	status = 'i';
	value_as_int = 0;
	value_as_float = 0.0;
}

void xvcalc_set_value(xv_number new_value)
{
	status = new_value.type;
	switch (status) {
	case 'i':
		value_as_int = new_value.i;
		value_as_float = (int) new_value.f;
		break;
	case 'f':
		value_as_int = (int) new_value.f;
		value_as_float = new_value.f;
	};
}

void xvcalc_set_malloc_error(void)
{
	status = 'm';
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
	status = 'e';
}
