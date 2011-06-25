#include "xvcalc.h"
#include "tokenize.h"
#include "xvcalcix.h"

int xvcalc_intx_yyparse(void);

static char   status         = '\0';
static int    value_as_int   = 0;
static float  value_as_float = 0.0;
static char * error_message  = NULL;

char xvcalc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = xvcalc_intx_yy_scan_string(inString);
	xvcalc_intx_yy_switch_to_buffer(buffer);
	xvcalc_intx_yyparse();
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

int xvcalc_intx_dice(int count, int faces)
{
	static int hasSeeded = 0;
	int i;
	int running = 0;
	if (!hasSeeded)
		srand(time(NULL));

	for (i = 1; i <= count; i++)
		running += (rand() % faces) + 1;

	return running;
}

void xvcalc_intx_set_int(int new_value)
{
	status = 'i';
	value_as_int = new_value;
	value_as_float = (float) new_value;
}

void xvcalc_intx_set_float(float new_value)
{
	status = 'f';
	value_as_int = (int) new_value;
	value_as_float = new_value;
}

void xvcalc_intx_set_value(int new_value)
{
	xvcalc_intx_set_int(new_value);
}

void xvcalc_intx_set_malloc_error(void)
{
	status = 'm';
}

void xvcalc_intx_report_lex_error(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		xvcalc_intx_set_malloc_error();
		return;
	}
	sprintf(message, t_message, bad_char);
	xvcalc_intx_yyerror(message);
	free(message);
}

void xvcalc_intx_yyerror(const char *s)
{
	if(!(error_message = malloc(strlen(s)+1))) {
		xvcalc_intx_set_malloc_error();
		return;
	}
	
        strcpy(error_message, s);
	status = 'e';
}
