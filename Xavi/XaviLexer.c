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

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */

#if USE_BISON
#include "XaviYyParser.h"
#else
#error "Building without BISON is currently not supported."
#endif

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

static int isIdLead(int character)
{
	return (
		isalpha(character)
		&& character != 'd');
}

static int isIdCharacter(int character)
{
	return (isalnum(character) || character == '_');
}

int XaviLexerRead(XaviLexer * lexer, YYSTYPE * token)
{
	XaviLexemeId terminal = L_EOI;
	int dfaState = DFA_START;
	size_t length = 0;
	free(lexer->lexeme);
	lexer->lexeme = NULL;

	// FIXME: This code doesn't check for pi or e.
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
			else if (isIdLead(*lexer->current)) {
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
		return EOL;
	case 'd':
	case '+':
	case '-':
	case '*':
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
		token->i = 0;
		return ERROR;
	}
}

XaviLexer * XaviLexerNew(const char * inputString)
{
	XaviLexer * rVal = malloc(sizeof(XaviLexer));
	rVal->input = inputString;
	rVal->begin = rVal->input; // Current beginning of read.
	rVal->current = rVal->input; // Current end of read.
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
