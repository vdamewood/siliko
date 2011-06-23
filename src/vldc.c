/*#include <stdlib.h>*/

#include "tokenizer.h"
#include "parserx.h"
#include "vldc.h"

int  vldc_yyparse(void);

int vldc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = vldc_yy_scan_string(inString);
	vldc_yy_switch_to_buffer(buffer);
	vldc_yyparse();
	return vldci_get_value();
}

char *vldc_error_message(void)
{
	return vldci_get_error_message();
}

int vldc_error_status(void)
{
	return vldci_get_error_status();
}

void vldc_clean(void)
{
	vldci_clean();
}
