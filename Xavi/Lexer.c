/*
 * Lexer.c: Lexical analyzer
 * Copyright 2012, 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "DataSource.h"
#include "Lexer.h"

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

struct Lexeme
{
	char *buffer;
	size_t current;
	size_t end;
};
typedef struct Lexeme Lexeme;

static void Append(Lexeme *Lex, char NewChar)
{
	char * tmp;

	if (Lex->current == Lex->end)
	{
		Lex->end++;
		tmp = realloc(Lex->buffer, Lex->end);
		if(tmp)
			Lex->buffer = tmp;
		else
			return;
	}

	Lex->buffer[Lex->current++] = NewChar;
}

void XaviLexerNext(XaviLexer *lexer)
{
	XaviDfaState dfaState = DFA_START;
	Lexeme Lex = {NULL, 0, 4};

	if (lexer->token.Type == ID)
		free(lexer->token.String);

	if (lexer->token.Type == EOL || lexer->token.Type == ERROR)
		return;

	// FIXME: Return a proper indication of memory error.
	Lex.buffer = malloc(Lex.end);

	while (dfaState != DFA_END)
	switch (dfaState)
	{
	case DFA_END:
		break;
	case DFA_START:
		if(isOperator(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			Append(&Lex, '\0');
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_TERM_CHAR;
		}
		else if (XaviDataSourceGet(lexer->source) == 'd')
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_DICE;
		}
		else if (XaviDataSourceGet(lexer->source) == 'e')
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_E;
		}
		else if (XaviDataSourceGet(lexer->source) == 'p')
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_PI_1;
		}
		else if (isdigit(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_INTEGER;
		}
		else if (isalpha(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_ID;
		}
		else if (isspace(XaviDataSourceGet(lexer->source)))
		{
			XaviDataSourceAdvance(lexer->source);
		}
		else if (XaviDataSourceGet(lexer->source) == '\0')
		{
			dfaState = DFA_TERM_EOI;
		}
		else
		{
			dfaState = DFA_ERROR;
		}
		break;
	case DFA_DICE:
		if (isalpha(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_CHAR;
		}
		break;
	case DFA_E:
		if (isalnum(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_E;
		}
		break;
	case DFA_PI_1:
		if (XaviDataSourceGet(lexer->source) == 'i')
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_PI_2;
		}
		else if (isIdCharacter(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_PI_2:
		if (isIdCharacter(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_ID;
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_PI;
		}
		break;
	case DFA_ID:
		if (isalnum(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_STRING;
		}
		break;
	case DFA_INTEGER:
		if (XaviDataSourceGet(lexer->source) == '.')
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
			dfaState = DFA_FLOAT;
		}
		else if (isdigit(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_INTEGER;
		}
		break;
	case DFA_FLOAT:
		if (isdigit(XaviDataSourceGet(lexer->source)))
		{
			Append(&Lex, XaviDataSourceGet(lexer->source));
			XaviDataSourceAdvance(lexer->source);
		}
		else
		{
			Append(&Lex, '\0');
			dfaState = DFA_TERM_FLOAT;
		}
		break;
	case DFA_TERM_INTEGER:
		lexer->token.Type = INTEGER;
		lexer->token.Integer = atoi(Lex.buffer);
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		lexer->token.Type = FLOAT;
		lexer->token.Float = (float)atof(Lex.buffer);
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		lexer->token.Type = FLOAT;
		lexer->token.Float = (float)EULER;
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		lexer->token.Type = FLOAT;
		lexer->token.Float = (float)PI;
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		lexer->token.Type = Lex.buffer[0];
		free(Lex.buffer);
		lexer->token.Integer = 0;
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		lexer->token.Type = ID;
		lexer->token.String = Lex.buffer;
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOI:
		lexer->token.Type = EOL;
		lexer->token.Integer = 0;
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		lexer->token.Type = ERROR;
		lexer->token.Integer = 0;
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	}
}

XaviLexer *XaviLexerNew(XaviDataSource *InputSource)
{
	XaviLexer *rVal = NULL;

	if (!(rVal = malloc(sizeof(XaviLexer))))
		return NULL;

	rVal->source = InputSource;
	rVal->token.Type = UNSET;
	rVal->token.Integer = 0;
	XaviLexerNext(rVal);

	return rVal;
}

void XaviLexerDestroy(XaviLexer *lexer)
{
	if (lexer)
	{
		if (lexer->token.Type == ID)
			free(lexer->token.String);
		XaviDataSourceDestroy(lexer->source);
		free(lexer);
	}
}

/*
XaviToken XaviLexerGetToken(XaviLexer *lexer)
{
	if (lexer->token == UNSET)
		XaviLexerLoad(lexer);

	return lexer->token;
}
*/

/*XaviToken XaviLexerGetToken(XaviLexer *lexer)
{
	if (lexer->token.Type == UNSET)
		XaviLexerNext(lexer);

	return lexer->token;
}*/
