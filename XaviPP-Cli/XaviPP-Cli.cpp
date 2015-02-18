/*
 * XaviPP-Cli.cpp: Command-line interface for XaviPP.
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

#include <XaviPP/FunctionCaller.hpp>
#include <XaviPP/InfixParser.hpp>

#include "StringSource.hpp"

char *readline(const char *prompt)
{
	size_t inputChar = 0;
	int currentPosition = 0;
	size_t bufSize = 10;
	char *rVal;
	char *newVal;

	fputs(prompt, stdout);
	fflush(stdout);

	if (!(rVal = (char*)malloc(bufSize)))
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
			newVal = (char *) realloc(rVal, bufSize);
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

	Xavi::FunctionCaller::SetUp();

	while(-1)
	{
		char *expression = readline(prompt);
		Xavi::SyntaxTreeNode *Tree;
		if(!expression)
			break;

		Tree = Xavi::ParseInfix(new StringSource(expression));
		free(expression);

		//MyParser.Parse();
		Xavi::Value result = Tree->Evaluate();
		delete Tree;
		Tree = 0;

		switch (result.Status())
		{
		case Xavi::Value::INTEGER:
			printf("%s%i\n", response, result.Integer());
			break;
		case Xavi::Value::FLOAT:
			printf("%s%f\n", response, result.Float());
			break;
		case Xavi::Value::MEMORY_ERR:
			printf("Out of memory.\n");
			break;
		case Xavi::Value::SYNTAX_ERR:
			printf("Syntax error.\n");
			break;
		case Xavi::Value::ZERO_DIV_ERR:
			printf("Division by zero error.\n");
			break;
		case Xavi::Value::BAD_FUNCTION:
			printf("Function not found.\n");
			break;
		case Xavi::Value::BAD_ARGUMENTS:
			printf("Bad argument count.\n");
			break;
		case Xavi::Value::DOMAIN_ERR:
			printf("Domain error.\n");
			break;
		case Xavi::Value::RANGE_ERR:
			printf("Range error.\n");
			break;
		}
	}

	Xavi::FunctionCaller::TearDown();

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
