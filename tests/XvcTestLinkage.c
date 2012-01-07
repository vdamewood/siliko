#include <stdio.h>
#include "XVCalc.h"

int main(int argc, char *argv[])
{
	XvcStatus status;
	char * expression;

	if (argc >= 2)
		expression = argv[1];
	else
		expression = "";

	XvcOpen();
	status = XvcParse(expression);
	switch (status) {
	case S_INTEGER:
		printf("Value: %i\n", XvcGetInteger());
		break;
	case S_FLOAT:
		printf("Value: %f\n", XvcGetFloat());
		break;
	case E_SYNTAX:
		printf("Syntax error.\n");
		break;
	case E_MEMORY:
		printf("Out of memory.\n");
		break;
	case E_ZERO_DIV:
		printf("Division by zero error.\n");
		break;
	case E_FUNCTION:
		printf("Function not found.\n");
		break;
	case E_ARGUMENTS:
		printf("Bad argument count.\n");
		break;
	case E_DOMAIN:
		printf("Domain error.\n");
		break;
	case E_RANGE:
		printf("Range error.\n");
		break;
	}
	XvcClose();
	return 0;
}
