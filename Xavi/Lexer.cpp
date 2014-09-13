/*
 * XaviLexer.c: Lexical analyzer
 * Copyright 2012, 2014 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include "Lexer.hpp"

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static int isOperator(int character)
{
	return
	(
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')'
	);
}

static int isIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

static char *lex_strdup(const char *string)
{
	char *rVal;

	if ((rVal = (char*) malloc(strlen(string) + 1)))
		strcpy(rVal, string);

	return rVal;
}

enum XaviDfaState
{
	DFA_ERROR = -1,
	DFA_START = 0,	/* "" */
	DFA_E,		/* "e" */
	DFA_PI_1,	/* "p" */
	DFA_PI_2,	/* "pi" */
	DFA_DICE,	/* "d" */
	DFA_ID,		/* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
	DFA_INTEGER,	/* [0-9]+ */
	DFA_FLOAT,	/* [0-9]+\.[0-9]+ */
	DFA_TERM_INTEGER,
	DFA_TERM_FLOAT,
	DFA_TERM_E,
	DFA_TERM_PI,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOI,
	DFA_END
};
typedef enum XaviDfaState XaviDfaState;

void Xavi::Lexer::Load(void)
{
	XaviDfaState dfaState = DFA_START;
	const char *current = location;
	std::string lexeme = std::string();

	if (token == EOL || token == ERROR)
		return;

	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if(isOperator(*current))
		{
			current++;
			dfaState = DFA_TERM_CHAR;
		}
		else if (*current == 'd')
		{
			lexeme += *current;
			current++;
			dfaState = DFA_DICE;
		}
		else if (*current == 'e')
		{
			lexeme += *current;
			current++;
			dfaState = DFA_E;
		}
		else if (*current == 'p')
		{
			lexeme += *current;
			current++;
			dfaState = DFA_PI_1;
		}
		else if (isdigit(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_INTEGER;
		}
		else if (isalpha(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_ID;
		}
		else if (isspace(*current))
		{
			location++;
			current++;
		}
		else if (*current == '\0')
		{
			dfaState = DFA_TERM_EOI;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (isalpha(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (isalnum(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (*current == 'i')
		{
			lexeme += *current;
			current++;
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(*current))
		{
			lexeme += *current;
			current++;
			dfaState = DFA_ID;
		}
		else
		{
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(*current))
		{
			lexeme += *current;
			current++;
		}
		else
		{
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (*current == '.')
		{
			lexeme += *current;
			current++;
			dfaState = DFA_FLOAT;
		}
		else if (isdigit(*current))
		{
			lexeme += *current;
			current++;
		}
		else
		{
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (isdigit(*current))
		{
			lexeme += *current;
			current++;
		}
		else
		{
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		token = INTEGER;
		value.i = atoi(lexeme.c_str());
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		token = FLOAT;
		value.f = atof(lexeme.c_str());
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		token = FLOAT;
		value.f = EULER;
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		token = FLOAT;
		value.f = PI;
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		token = (TokenType) *location;
		value.i = 0;
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		token = ID;
		value.s =  lex_strdup(lexeme.c_str());
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOI:
		token = EOL;
		value.i = 0;
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		token = ERROR;
		value.i = 0;
		dfaState = DFA_END;
		break;
	}
	location = current;
}

Xavi::Lexer::Lexer(const char * InputString)
{
	location = InputString;
	token = UNSET;
	value.i = 0;
}

Xavi::Lexer::~Lexer(void)
{
	if (token == ID)
		free(value.s);
}

Xavi::TokenType Xavi::Lexer::GetToken(void)
{
	if (token == UNSET)
		Load();

	return token;
}

Xavi::TokenValue Xavi::Lexer::GetValue(void)
{
	TokenValue rVal;
	if (token == UNSET)
		Load();

	switch (token)
	{
	case INTEGER:
		rVal.i = value.i;
		break;
	case FLOAT:
		rVal.f = value.f;
		break;
	case ID:
		rVal.s = value.s;
		break;
	default:
		rVal.i = token;
	}
	return rVal;
}

void Xavi::Lexer::Next(void)
{
	if (token == ID)
		free(value.s);

	if (token != EOL && token != ERROR)
	{
		token = UNSET;
		value.i = 0;
	}
}
