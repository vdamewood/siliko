#include "tokenizer.h"
#include "parserx.h"

int vldc(char *inString)
{
	YY_BUFFER_STATE buffer;
	buffer = vldc_scan_string(inString);
	vldc_switch_to_buffer(buffer);
	vldcparse();
	return getvalue();
}
