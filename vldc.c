#include <stdlib.h>

#include "tokenizer.h"
#include "parserx.h"
#include "vldc.h"

static char *error_message = NULL;

int vldc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = vldc_scan_string(inString);
	vldc_switch_to_buffer(buffer);
	vldcparse();
	return getvalue();
}

char *vldc_error(void)
{
	return error_message;
}

void vldc_clean(void)
{
	free(error_message);
}

