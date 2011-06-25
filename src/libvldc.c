#include "tokenizer.h"
#include "parserx.h"
#include "libvldc.h"

int vldc_yyparse(void);

static char   status         = '\0';
static int    value_as_int   = 0;
static float  value_as_float = 0.0;
static char * error_message  = NULL;

char vldc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = vldc_yy_scan_string(inString);
	vldc_yy_switch_to_buffer(buffer);
	vldc_yyparse();
	return status;
}

int vldc_get_int(void)
{
	return value_as_int;
}

float vldc_get_float(void)
{
	return value_as_float;
}

char *vldc_error_message(void)
{
        return error_message;
}

void vldc_clean(void)
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

int vldci_dice(int count, int faces)
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

void vldci_set_int(int new_value)
{
	status = 'i';
	value_as_int = new_value;
	value_as_float = (float) new_value;
}

void vldci_set_float(float new_value)
{
	status = 'f';
	value_as_int = (int) new_value;
	value_as_float = new_value;
}

void vldci_set_value(int new_value)
{
	vldci_set_int(new_value);
}

void vldci_set_malloc_error(void)
{
	status = 'm';
}

void vldci_report_lex_error(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		vldci_set_malloc_error();
		return;
	}
	sprintf(message, t_message, bad_char);
	vldc_yyerror(message);
	free(message);
}

void vldc_yyerror(const char *s)
{
	if(!(error_message = malloc(strlen(s)+1))) {
		vldci_set_malloc_error();
		return;
	}
	
        strcpy(error_message, s);
	status = 'e';
}
