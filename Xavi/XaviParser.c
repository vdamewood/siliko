/*
 * XaviParser.c: Parser for Xavi.
 * Copyright 2014 Vincent Damewood
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

#include "Xavi.h"
#include "XaviStructs.h"
#include "XaviParser.h"

#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviFunctionId.h"

static XaviTree * GetExpr0(XaviLexer * lexer);
static XaviTree * GetExpr0r(XaviLexer * lexer);
static XaviTree * GetExpr1(XaviLexer * lexer);
static XaviTree * GetExpr1r(XaviLexer * lexer);
static XaviTree * GetExpr2(XaviLexer * lexer);
static XaviTree * GetExpr2lf(XaviLexer * lexer);
static XaviTree * GetExpr3(XaviLexer * lexe);
static XaviTree * GetExpr3lf(XaviLexer * lexer);
static XaviTree * GetAtom(XaviLexer * lexer);
static XaviTree * GetNumber(XaviLexer * lexer);
static XaviTree * GetUNumber(XaviLexer * lexer);
static XaviTree * GetFCall(XaviLexer * lexer);
static XaviArglist * GetArglist(XaviLexer * lexer);
static XaviArglist * GetArglistLF(XaviLexer * lexer);

static XaviTree * GetExpr0(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * rest;

	value = GetExpr1(lexer);
	if (value == NULL)
		return NULL;
	rest = GetExpr0r(lexer);

	if (rest == NULL)
		return value;
	else
	{
		XaviTreeGraftLeft(rest, value);
		return rest;
	}
}

static XaviTree * GetExpr0r(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * rest;
	int operator;

	switch (XaviLexerGetToken(lexer))
	{
		case '+':
			operator = OP_ADD;
			break;
		case '-':
			operator = OP_SUB;
			break;
		default:
			return NULL;
	}

	XaviLexerNext(lexer);
	value = GetExpr1(lexer);
	rest = GetExpr0r(lexer);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(
			rest,
			XaviTreeNewOperator(operator, NULL, value));
		return rest;
	}
	else
	{
		return XaviTreeNewOperator(operator, NULL, value);
	}
}

static XaviTree * GetExpr1(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * rest;

	value = GetExpr2(lexer);
	if (value == NULL)
		return NULL;
	rest = GetExpr1r(lexer);

	if (rest == NULL)
		return value;
	else
	{
		XaviTreeGraftLeft(rest, value);
		return rest;
	}
}

static XaviTree * GetExpr1r(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * rest;
	int operator;

	switch (XaviLexerGetToken(lexer))
	{
		case '*':
			operator = OP_MUL;
			break;
		case '/':
			operator = OP_DIV;
			break;
		default:
			return NULL;
	}

	XaviLexerNext(lexer);
	value = GetExpr2(lexer);
	rest = GetExpr1r(lexer);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(
			rest,
			XaviTreeNewOperator(operator, NULL, value));
		return rest;
	}
	else
	{
		return XaviTreeNewOperator(operator, NULL, value);
	}
}

static XaviTree * GetExpr2(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * leftFactor;

	value = GetExpr3(lexer);
	if (value == NULL)
		return NULL;
	leftFactor = GetExpr2lf(lexer);

	if (leftFactor == NULL)
		return value;
	else
		return XaviTreeNewOperator(OP_POW, value, leftFactor);
}

static XaviTree * GetExpr2lf(XaviLexer * lexer)
{
	if (XaviLexerGetToken(lexer) == '^')
	{
		XaviLexerNext(lexer);

		switch (XaviLexerGetToken(lexer))
		{
		case INTEGER:
		case FLOAT:
		case '-':
		case ID:
		case '(':
			return GetExpr2(lexer);
		default:
			return XaviTreeNewSyntaxError();
		}
	}
	else
	{
		return NULL;
	}
}

static XaviTree * GetExpr3(XaviLexer * lexer)
{
	XaviTree * value;
	XaviTree * dice;

	value = GetAtom(lexer);
	dice = GetExpr3lf(lexer);

	return (dice == NULL)
		? value
		: XaviTreeNewOperator(OP_DICE, value, dice);
}

static XaviTree * GetExpr3lf(XaviLexer * lexer)
{
	XaviTokenValue value;

	if(XaviLexerGetToken(lexer) == 'd')
	{
		XaviLexerNext(lexer);
		if (XaviLexerGetToken(lexer) != INTEGER)
			return XaviTreeNewSyntaxError();

		value = XaviLexerGetValue(lexer);
		XaviLexerNext(lexer);
		return XaviTreeNewInteger(value.i);
	}
	else
	{
		return NULL;
	}
}

static XaviTree * GetAtom(XaviLexer * lexer)
{
	XaviTree * value;

	switch(XaviLexerGetToken(lexer))
	{
		case '-':
		case INTEGER:
		case FLOAT:
			return GetNumber(lexer);
		case '(':
			XaviLexerNext(lexer);
			value = GetExpr0(lexer);
			if (XaviLexerGetToken(lexer) != ')')
				return NULL;
			XaviLexerNext(lexer);
			return value;
		case ID:
			return GetFCall(lexer);
		default:
			return NULL;
	}
}

static XaviTree * GetNumber(XaviLexer * lexer)
{
	switch (XaviLexerGetToken(lexer))
	{
		case INTEGER:
		case FLOAT:
			return GetUNumber(lexer);
		case '-':
			XaviLexerNext(lexer);
			XaviTree * Symbol2 = GetUNumber(lexer);
			if (Symbol2 == NULL || !XaviTreeNegate(Symbol2))
				return NULL;

			return Symbol2;
		default:
			return NULL;
	}
}

static XaviTree * GetUNumber(XaviLexer * lexer)
{
	XaviTokenValue value;

	switch (XaviLexerGetToken(lexer))
	{
		case INTEGER:
			value = XaviLexerGetValue(lexer);
			XaviLexerNext(lexer);
			return XaviTreeNewInteger(value.i);
		case FLOAT:
			value = XaviLexerGetValue(lexer);
			XaviLexerNext(lexer);
			return XaviTreeNewFloat(value.f);
		default:
			return NULL;
	}
}

static XaviTree * GetFCall(XaviLexer * lexer)
{
	int token;
	XaviTokenValue value;
	char * id;
	int collapsedArgc;
	XaviTree ** collapsedArgv;
	XaviArglist * arglist;

	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);
	id = XaviFunctionIdNew(value.s);

	XaviLexerNext(lexer);
	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);
	
	XaviLexerNext(lexer);
	if (token != '(')
		return NULL;

	arglist = GetArglist(lexer);

	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);
	XaviLexerNext(lexer);
	if (token != ')')
	{
		XaviFunctionIdDelete(id);
		XaviArglistDelete(arglist);
		return NULL;
	}

	if(arglist) {
		collapsedArgc = arglist->depth;
		collapsedArgv = XaviArglistGetTrees(arglist);
		XaviArglistDissolve(arglist);
	}
	else {
		collapsedArgc = 0;
		collapsedArgv = NULL;
	}
	
	return XaviTreeNewFunction(id, collapsedArgc, collapsedArgv);
}

static XaviArglist * GetArglist(XaviLexer * lexer)
{
	XaviTree * expression;
	XaviArglist * subArglist;

	expression = GetExpr0(lexer);
	subArglist = GetArglistLF(lexer);

	return XaviArglistNew(expression, subArglist);
}

static XaviArglist * GetArglistLF(XaviLexer * lexer)
{
	if (XaviLexerGetToken(lexer) != ',')
	{
		return NULL;
	}
	else
	{
		XaviLexerNext(lexer);
		return GetArglist(lexer);
	}
}

XaviNumber XaviInternalParse(XaviLexer * lexer)
{
	XaviNumber value;
	
	if (XaviLexerGetToken(lexer) == EOL)
	{
		value.status = S_INTEGER;
		value.i = 0;
		return value;
	}

	XaviTree * tree = GetExpr0(lexer);

	if (tree != NULL && tree->type != 'e' && XaviLexerGetToken(lexer) == EOL)
	{
		value = XaviTreeEvaluate(tree);
	}
	else
	{
		value.status = E_SYNTAX;
		value.i = 0;
	}
	XaviTreeDelete(tree);
	
	return value;
}
