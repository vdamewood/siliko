/*
 * XaviTestLinkage.c: Program to test linking to Xavi.
 * Copyright 2012 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "Xavi.h"

int main(int argc, char *argv[])
{
	char * expression;
	XaviNumber value;

	if (argc >= 2)
		expression = argv[1];
	else
		expression = "";

	XaviOpen();
	value = XaviParse(expression);
	switch (value.status) {
	case S_INTEGER:
		printf("Value: %i\n", value.i);
		break;
	case S_FLOAT:
		printf("Value: %f\n", value.f);
		break;
	case E_SYNTAX:
		printf("Syntax error.\n");
		break;
	case E_INTERNAL:
		printf("Internal error.\n");
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
	XaviClose();
	return 0;
}
