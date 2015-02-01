/*
 * Xavi-Cli.c: Command-line interface for Xavi
 * Copyright 2012, 2014, 2015 Vincent Damewood
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
#include <stdlib.h>

#if defined XAVI_USE_UNIX
#	include <unistd.h>
#	define ISATTY() (isatty(fileno(stdin)) && isatty(fileno(stdout)))
#else
#	define ISATTY() (-1)
#endif

#include <Xavi/Xavi.h>

char *readline(const char *prompt)
{
	size_t inputChar = 0;
	int currentPosition = 0;
	size_t bufSize = 10;
	char *rVal;
	char *newVal;

	fputs(prompt, stdout);
	fflush(stdout);

	if (!(rVal = malloc(bufSize)))
	{
		return NULL;
	}

	while(-1)
	{
		inputChar = fgetc(stdin);

		if (inputChar == EOF)
		{
			free(rVal);
			rVal = NULL;
			break;
		}
		else if (inputChar == '\n')
		{
			rVal[currentPosition] = '\0';
			break;
		}

		rVal[currentPosition++] = (char) inputChar;

		if(currentPosition == bufSize)
		{
			bufSize += 10;
			newVal = realloc(rVal, bufSize);
			if (newVal)
			{
				rVal = newVal;
			}
			else
			{
				free(rVal);
				rVal = NULL;
				break;
			}
		}
	}

	return rVal;
}


int main(int argc, char *argv[])
{
	char *expression;
	XaviResult value;
	const char *prompt;
	const char *response;

	if (ISATTY())
	{
		prompt = "> ";
		response = "= ";
	}
	else
	{
		prompt = "";
		response = "";
	}

	XaviOpen();

	while(-1)
	{
		expression = readline(prompt);

		if(!expression)
			break;

		value = XaviEvaluate(expression);
		free(expression);

		switch (value.status)
		{
		case XAVI_RS_INTEGER:
			printf("%s%i\n", response, value.i);
			break;
		case XAVI_RS_FLOAT:
			printf("%s%f\n", response, value.f);
			break;
		case XAVI_RE_MEMORY:
			printf("Out of memory.\n");
			break;
		case XAVI_RE_SYNTAX:
			printf("Syntax error.\n");
			break;
		case XAVI_RE_ZERO_DIV:
			printf("Division by zero error.\n");
			break;
		case XAVI_RE_FUNCTION:
			printf("Function not found.\n");
			break;
		case XAVI_RE_ARGUMENTS:
			printf("Bad argument count.\n");
			break;
		case XAVI_RE_DOMAIN:
			printf("Domain error.\n");
			break;
		case XAVI_RE_RANGE:
			printf("Range error.\n");
			break;
		}
	}

	XaviClose();

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
