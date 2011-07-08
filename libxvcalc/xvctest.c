#include <stdio.h>
#include "xvcalc.h"

int main(int argc, char *argv[])
{
	char status;
	char * expression;
	int int_val;
	float flt_val;

	if (argc >= 2)
		expression = argv[1];
	else
		expression = "";

	status = xvcalc(expression);
	if (status == 'i' || status == 'f') {
		int_val = xvcalc_get_int();
		flt_val = xvcalc_get_float();
	}
	else if (status == 'm') printf("Out of memory.");
	else if (status == 'e') printf("%s\n", xvcalc_error_message());
	else printf("Oops\n");
	xvcalc_clean();

	printf("%c: %d %f\n", status, int_val, flt_val);
	return 0;
}
