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
	L_FLOAT,			/* [0-9]+\.[0-9]+ */
	L_ID,				/* [a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]* */
	L_E,				/* "e" */
	L_PI,				/* "pi" */
	L_ERROR
};

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

typedef enum XaviLexemeId XaviLexemeId;

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


int XaviLexerRead(XaviLexer * lexer, XaviTokenValue * token)
{
	int rVal = XaviLexerPeek(lexer, token);
	XaviLexerNext(lexer);
	return rVal;
}

static void XaviLexerLoad(XaviLexer *);

int XaviLexerPeek(XaviLexer * lexer, XaviTokenValue * token)
{
	if (!lexer->token)
		XaviLexerLoad(lexer);

	switch (lexer->token)
	{
	case INTEGER:
		token->i = lexer->intValue;
		break;
	case FLOAT:
		token->f = lexer->floatValue;
		break;
	case ID:
		token->s = lexer->lexeme;
		break;
	}
	return lexer->token;
	
}

void XaviLexerNext(XaviLexer * lexer)
{
	lexer->token = 0;
	lexer->intValue = 0;
	lexer->lexeme = NULL;
}

static void XaviLexerLoad(XaviLexer * lexer)
{
	XaviLexemeId terminal = L_EOI;
	int dfaState = DFA_START;
	size_t length = 0;
	free(lexer->lexeme);
	lexer->lexeme = NULL;

	while (dfaState != -1) {
		switch (dfaState) {
		case DFA_START:
			if(isOperator(*lexer->current)) {
				lexer->current++;
				terminal = *lexer->begin;
				dfaState = DFA_TERM_CHAR;
			}
			else if (*lexer->current == 'd') {
				lexer->current++;
				dfaState = DFA_DICE;
			}
			else if (*lexer->current == 'e') {
				lexer->current++;
				dfaState = DFA_E;
			}
			else if (*lexer->current == 'p') {
				lexer->current++;
				dfaState = DFA_PI_1;
			}
			else if (isdigit(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_INTEGER;
			}
			else if (isalpha(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_ID;
			}
			else if (isspace(*lexer->current)) {
				lexer->begin++;
				lexer->current++;
			}
			else if (*lexer->current == '\0'){
				dfaState = DFA_TERM_EOI;
			}
			else {
				dfaState = DFA_TERM_ERROR;
			}
			break;
		case DFA_DICE:
			if (isalpha(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = *lexer->begin;
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_E:
			if (isalnum(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_E;
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_PI_1:
			if (*lexer->current == 'i') {
				lexer->current++;
				dfaState = DFA_PI_2;
			}
			else if (isIdCharacter(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_ID;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_PI_2:
			if (isIdCharacter(*lexer->current)) {
				lexer->current++;
				dfaState = DFA_ID;
			}
			else {
				terminal = L_PI;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_ID:
			if (isalnum(*lexer->current)) {
				lexer->current++;
			}
			else {
				terminal = L_ID;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_INTEGER:
			if (*lexer->current == '.') {
				lexer->current++;
				dfaState = DFA_FLOAT;
			}
			else if (isdigit(*lexer->current)) {
				*lexer->current++;
			}
			else {
				terminal = L_INTEGER,
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_FLOAT:
			if (isdigit(*lexer->current)) {
				lexer->current++;
			}
			else {
				terminal = L_FLOAT;
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_TERM_CHAR:
			lexer->lexeme = malloc(2);
			lexer->lexeme[0] = *lexer->begin;
			lexer->lexeme[1] = '\0';
			lexer->begin = lexer->current;
			dfaState = -1;
			break;
		case DFA_TERM_STRING:
			length = lexer->current - lexer->begin;
			lexer->lexeme = malloc(length+1);
			strncpy(lexer->lexeme, lexer->begin, length);
			lexer->lexeme[length] = '\0';			
			lexer->begin = lexer->current;
			dfaState = -1;
			break;
		case DFA_TERM_EOI:
			terminal = EOL;
			dfaState = -1;
			break;
		case DFA_TERM_ERROR:
			terminal = -1;
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
		lexer->token = *lexer->lexeme;
		break;
	case L_INTEGER:
		lexer->intValue = atoi(lexer->lexeme);
		lexer->token = INTEGER;
		break;
	case L_FLOAT:
		lexer->floatValue = atof(lexer->lexeme);
		lexer->token = FLOAT;
		break;
	case L_E:
		lexer->floatValue = EULER;
		lexer->token = FLOAT;
		break;
	case L_PI:
		lexer->floatValue = PI;
		lexer->token = FLOAT;
		break;
	case L_ID:
		lexer->token = ID;
		break;
	default:
		lexer->token = ERROR;
	}
}

XaviLexer * XaviLexerNew(const char * inputString)
{
	XaviLexer * rVal = malloc(sizeof(XaviLexer));
	rVal->input = inputString;
	rVal->begin = rVal->input; // Current beginning of read.
	rVal->current = rVal->input; // Current end of read.
	rVal->lexeme = NULL;
	rVal->token = 0;
	rVal->intValue = 0;
	return rVal;
}

void XaviLexerDestroy(XaviLexer** theLexer)
{
	XaviLexer * garbage = *theLexer;
	free(garbage->lexeme);
	free(*theLexer);
	*theLexer = NULL;
}
