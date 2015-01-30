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

static void XaviLexerLoad(XaviLexer *lexer)
{
	XaviDfaState dfaState = DFA_START;
	Lexeme Lex = {NULL, 0, 4};

	if (lexer->token == EOL || lexer->token == ERROR)
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
		lexer->token = INTEGER;
		lexer->value.i = atoi(Lex.buffer);
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_FLOAT:
		lexer->token = FLOAT;
		lexer->value.f = atof(Lex.buffer);
		free(Lex.buffer);
		dfaState = DFA_END;
		break;
	case DFA_TERM_E:
		lexer->token = FLOAT;
		lexer->value.f = EULER;
		dfaState = DFA_END;
		break;
	case DFA_TERM_PI:
		lexer->token = FLOAT;
		lexer->value.f = PI;
		dfaState = DFA_END;
		break;
	case DFA_TERM_CHAR:
		lexer->token = Lex.buffer[0];
		free(Lex.buffer);
		lexer->value.i = 0;
		dfaState = DFA_END;
		break;
	case DFA_TERM_STRING:
		lexer->token = ID;
		lexer->value.s = Lex.buffer;
		dfaState = DFA_END;
		break;
	case DFA_TERM_EOI:
		lexer->token = EOL;
		lexer->value.i = 0;
		dfaState = DFA_END;
		break;
	case DFA_ERROR:
		lexer->token = ERROR;
		lexer->value.i = 0;
		dfaState = DFA_END;
		break;
	}
}

struct XaviStringSourceState
{
	char *string;
	char *current;
};

typedef struct XaviStringSourceState XaviStringSourceState;

static int XaviStringSourceAdvance(void *State)
{
	if (((XaviStringSourceState *)State)->current)
	{
		((XaviStringSourceState *)State)->current++;
		return -1;
	}
	else
	{
		return 0;
	}
}

static char XaviStringSourceGet(void *State)
{
	return *((XaviStringSourceState *)State)->current;
}

static void XaviStringSourceDestroy(void *State)
{
	free(((XaviStringSourceState *)State)->string);
	free(State);
}


XaviLexer *XaviLexerNew(const char *inputString)
{
	XaviLexer *rVal = NULL;
	XaviStringSourceState *state = NULL;
	XaviDataSource *source = NULL;

	if (!(state = malloc(sizeof(XaviStringSourceState))))
		goto memerr;

	state->string = strdup(inputString);
	state->current = state->string;

	if (!(source = malloc(sizeof(XaviDataSource))))
		goto memerr;

	source->State = state;
	source->AdvanceFunction = XaviStringSourceAdvance;
	source->GetFunction = XaviStringSourceGet;
	source->DestroyFunction = XaviStringSourceDestroy;

	if (!(rVal = malloc(sizeof(XaviLexer))))
		goto memerr;

	rVal->source = source;
	rVal->token = UNSET;
	rVal->value.i = 0;

	return rVal;
memerr:
	free(rVal);
	free(source);
	free(state);

	return NULL;
}

void XaviLexerDestroy(XaviLexer **theLexer)
{
	XaviLexer *garbage = *theLexer;
	if (garbage)
	{
		if (garbage->token == ID)
			free(garbage->value.s);
		free((*theLexer)->source);
		free(*theLexer);
	}
	*theLexer = NULL;
}

XaviToken XaviLexerGetToken(XaviLexer *lexer)
{
	if (lexer->token == UNSET)
		XaviLexerLoad(lexer);

	return lexer->token;
}

XaviTokenValue XaviLexerGetValue(XaviLexer *lexer)
{
	XaviTokenValue rVal;
	if (lexer->token == UNSET)
		XaviLexerLoad(lexer);

	switch (lexer->token)
	{
	case INTEGER:
		rVal.i = lexer->value.i;
		break;
	case FLOAT:
		rVal.f = lexer->value.f;
		break;
	case ID:
		rVal.s = lexer->value.s;
		break;
	default:
		rVal.i = lexer->token;
	}
	return rVal;
}

void XaviLexerNext(XaviLexer *lexer)
{
	if (lexer->token == ID)
		free(lexer->value.s);

	if (lexer->token != EOL && lexer->token != ERROR)
	{
		lexer->token = UNSET;
		lexer->value.i = 0;
	}
}
