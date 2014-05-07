/*
 * XaviLexer.c: Hand-written lexical analyzer.
 * Copyright 2012 Vincent Damewood
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


#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "XaviLexer.h"

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

enum XaviLexemeId
{
	L_EOI = 0,
	L_INTEGER = 257,	/* [0-9]+ */
	L_FLOAT,		/* [0-9]+\.[0-9]+ */
	L_ID,			/* [a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]* */
	L_E,			/* "e" */
	L_PI,			/* "pi" */
	L_ERROR
};
typedef enum XaviLexemeId XaviLexemeId;

enum XaviDfaState
{
	DFA_END = -1,
	DFA_START = 0,
	DFA_E,
	DFA_PI_1,
	DFA_PI_2,
	DFA_DICE,
	DFA_ID,
	DFA_INTEGER,
	DFA_FLOAT,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOI,
	DFA_TERM_ERROR
};
typedef enum XaviDfaState XaviDfaState;

static int isOperator(int character)
{
	return (
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')');
}

static int isIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

static void XaviLexerLoad(XaviLexer * lexer)
{
	XaviLexemeId terminal = L_EOI;
	XaviDfaState dfaState = DFA_START;
	size_t length = 0;
	char * lexeme;
	const char * current = lexer->location;

	while (dfaState != DFA_END) {
		switch (dfaState) {
		case DFA_START:
			if(isOperator(*current)) {
				current++;
				terminal = *lexer->location;
				dfaState = DFA_TERM_CHAR;
			}
			else if (*current == 'd') {
				current++;
				dfaState = DFA_DICE;
			}
			else if (*current == 'e') {
				current++;
				dfaState = DFA_E;
			}
			else if (*current == 'p') {
				current++;
				dfaState = DFA_PI_1;
			}
			else if (isdigit(*current)) {
				current++;
				dfaState = DFA_INTEGER;
			}
			else if (isalpha(*current)) {
				current++;
				dfaState = DFA_ID;
			}
			else if (isspace(*current)) {
				lexer->location++;
				current++;
			}
			else if (*current == '\0'){
				dfaState = DFA_TERM_EOI;
			}
			else {
				dfaState = DFA_TERM_ERROR;
			}
			break;
		case DFA_DICE:
			if (isalpha(*current)) {
				current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = *lexer->location;
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_E:
			if (isalnum(*current)) {
				current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_E;
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_PI_1:
			if (*current == 'i') {
				current++;
				dfaState = DFA_PI_2;
			}
			else if (isIdCharacter(*current)) {
				current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_ID;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_PI_2:
			if (isIdCharacter(*current)) {
				current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_PI;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_ID:
			if (isalnum(*current)) {
				current++;
			}
			else {
				terminal = L_ID;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_INTEGER:
			if (*current == '.') {
				current++;
				dfaState = DFA_FLOAT;
			}
			else if (isdigit(*current)) {
				*current++;
			}
			else {
				terminal = L_INTEGER,
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_FLOAT:
			if (isdigit(*current)) {
				current++;
			}
			else {
				terminal = L_FLOAT;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_TERM_CHAR:
			lexeme = malloc(2);
			lexeme[0] = *lexer->location;
			lexeme[1] = '\0';
			lexer->location = current;
			dfaState = DFA_END;
			break;
		case DFA_TERM_STRING:
			length = current - lexer->location;
			lexeme = malloc(length+1);
			strncpy(lexeme, lexer->location, length);
			lexeme[length] = '\0';			
			lexer->location = current;
			dfaState = DFA_END;
			break;
		case DFA_TERM_EOI:
			terminal = EOL;
			dfaState = DFA_END;
			break;
		case DFA_TERM_ERROR:
			terminal = DFA_END;
			break;
		}
	}

	switch (terminal) {
	case L_EOI:
		lexer->token = EOL;
		break;
	case 'd':
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case ',':
	case '(':
	case ')':
		lexer->token = *lexeme;
		free(lexeme);
		break;
	case L_INTEGER:
		lexer->value.i = atoi(lexeme);
		lexer->token = INTEGER;
		free(lexeme);
		break;
	case L_FLOAT:
		lexer->value.f = atof(lexeme);
		lexer->token = FLOAT;
		free(lexeme);
		break;
	case L_E:
		lexer->value.f = EULER;
		lexer->token = FLOAT;
		free(lexeme);
		break;
	case L_PI:
		lexer->value.f = PI;
		lexer->token = FLOAT;
		free(lexeme);
		break;
	case L_ID:
		lexer->token = ID;
		lexer->value.s = lexeme;
		break;
	default:
		lexer->token = ERROR;
	}
}

XaviLexer * XaviLexerNew(const char * inputString)
{
	XaviLexer * rVal = malloc(sizeof(XaviLexer));
	rVal->input = inputString;
	rVal->location = rVal->input;
	rVal->token = UNSET;
	rVal->value.i = 0;
	return rVal;
}

void XaviLexerDestroy(XaviLexer** theLexer)
{
	XaviLexer * garbage = *theLexer;
	if (garbage->token == ID)
		free(garbage->value.s);
	free(*theLexer);
	*theLexer = NULL;
}

XaviToken XaviLexerGetToken(XaviLexer * lexer)
{
	if (lexer->token == UNSET)
		XaviLexerLoad(lexer);

	return lexer->token;
}

XaviTokenValue XaviLexerGetValue(XaviLexer * lexer)
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
	}
	return rVal;
}

void XaviLexerNext(XaviLexer * lexer)
{
	if (lexer->token == ID)
		free(lexer->value.s);
	lexer->token = UNSET;
	lexer->value.i = 0;
}
