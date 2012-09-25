/* This file is part of Xavi. Copyright 2012 Vincent Damewood. Licensed under
   the GNU Lesser General Public License, version 3. */

#include <stdlib.h>
#include "XaviLexer.h"

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */

#if USE_BISON
#include "XaviYyParser.h"
#else
#error "Building without BISON is currently not supported."
#endif

#define EULER 2.18
#define PI 3.14

enum XaviLexemeId
{
	L_EOI = 0,
	L_INTEGER = 257,	/* [0-9]+ */
	L_FLOAT,			/* [0-9]+\.[0-9]+ */
	L_ID,				/* [a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]* */
	L_E,				/* "e" */
	L_PI				/* "pi" */
};

typedef enum XaviLexemeId XaviLexemeId;

int XaviLexerRead(yyscan_t inLexer, YYSTYPE * token)
{
	XaviLexer * lexer = (XaviLexer *) inLexer;
	XaviLexemeId terminal = L_EOI;

	/* TODO: Figure out which terminal we have and assign it to terminal
			Then assign the lexeme to lexer->lexeme. */

	switch (terminal) {
	case L_EOI:
		return EOL;
	case 'd':
	case '+':
	case '-':
	case '/':
	case '^':
	case ',':
	case '(':
	case ')':
		return *lexer->lexeme;
	case L_INTEGER:
		token->i = atoi(lexer->lexeme);
		return INTEGER;
	case L_FLOAT:
		token->f = atof(lexer->lexeme);
		return FLOAT;
	case L_E:
		token->f = EULER;
		return FLOAT;
	case L_PI:
		token->f = PI;
		return FLOAT;
	case L_ID:
		token->s = lexer->lexeme;
		return ID;
	default:
		token->s = lexer->lexeme;
		return ERROR;
	}
}

XaviLexer * XaviLexerNew(const char * inputString)
{
	XaviLexer * rVal = malloc(sizeof(XaviLexer));
	rVal->input = inputString;
	rVal->current = rVal->input;
	rVal->lexeme = NULL;
	return rVal;
}

void XaviLexerDestroy(XaviLexer** theLexer)
{
	XaviLexer * garbage = *theLexer;
	free(garbage->lexeme);
	free(*theLexer);
	*theLexer = NULL;
}
