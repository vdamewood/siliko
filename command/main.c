/* Siliko.c: Command-line interface for Siliko
 * Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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
#include <string.h>

#if defined USE_UNIX
#	include <unistd.h>
#	define ISATTY() (isatty(fileno(stdin)) && isatty(fileno(stdout)))
#else
#	define ISATTY() (-1)
#endif

#include "FunctionCaller.h"
#include "InfixParser.h"
#include "Value.h"
#include "StringSource.h"

#if HAVE_READLINE
#include <readline/readline.h>
#else
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

void add_history(char *command)
{
}
#endif /* HAVE_READLINE */

int main(int argc, char *argv[])
{
	SilikoValue value;
	SilikoSyntaxTreeNode *tree;
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

	SilikoFunctionCallerSetUp();

	char *expression = NULL;
	char *old_expression = NULL;

	while(-1)
	{
		expression = readline(prompt);

		if(!expression)
		{
			free(old_expression);
			old_expression = NULL;
			break;
		}

		if(*expression && (!old_expression || strcmp(expression, old_expression) != 0))
			add_history(expression);

		free(old_expression);
		old_expression = NULL;

		tree = SilikoParseInfix(
			SilikoStringSourceNew(expression));
		value = SilikoSyntaxTreeEvaluate(tree);
		SilikoSyntaxTreeDelete(tree);

		char *ResultString = SilikoValueToString(value);
		printf("%s\n", ResultString);
		free(ResultString);

		old_expression = expression;
	}

	SilikoFunctionCallerTearDown();

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
