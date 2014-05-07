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

#include "XaviCleanup.h"
#include "XaviEvaluate.h"
#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviFunctionId.h"

static XaviTree * GetExpr0(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr0r(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr1(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr1r(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr2(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr2lf(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr3(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetExpr3lf(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetAtom(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetNumber(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetUNumber(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviTree * GetFCall(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviArglist * GetArglist(XaviLexer * lexer, XaviMemoryPool * pool);
static XaviArglist * GetArglistLF(XaviLexer * lexer, XaviMemoryPool * pool);

static XaviTree * GetExpr0(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * rest;

	value = GetExpr1(lexer, pool);
	if (value == NULL)
		return NULL;
	rest = GetExpr0r(lexer, pool);

	if (rest == NULL)
		return value;
	else
	{
		XaviTreeGraftLeft(rest, value, pool);
		return rest;
	}
}

static XaviTree * GetExpr0r(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * rest;
	int operator;

	switch (XaviLexerPeek(lexer, NULL))
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
	value = GetExpr1(lexer, pool);
	rest = GetExpr0r(lexer, pool);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(
			rest,
			XaviTreeNewOperator(operator, NULL, value, pool),
			pool);
		return rest;
	}
	else
	{
		return XaviTreeNewOperator(operator, NULL, value, pool);
	}
}

static XaviTree * GetExpr1(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * rest;

	value = GetExpr2(lexer, pool);
	if (value == NULL)
		return NULL;
	rest = GetExpr1r(lexer, pool);

	if (rest == NULL)
		return value;
	else
	{
		XaviTreeGraftLeft(rest, value, pool);
		return rest;
	}
}

static XaviTree * GetExpr1r(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * rest;
	int operator;

	switch (XaviLexerPeek(lexer, NULL))
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
	value = GetExpr2(lexer, pool);
	rest = GetExpr1r(lexer, pool);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(
			rest,
			XaviTreeNewOperator(operator, NULL, value, pool),
			pool);
		return rest;
	}
	else
	{
		return XaviTreeNewOperator(operator, NULL, value, pool);
	}
}

static XaviTree * GetExpr2(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * leftFactor;

	value = GetExpr3(lexer, pool);
	if (value == NULL)
		return NULL;
	leftFactor = GetExpr2lf(lexer, pool);

	if (leftFactor == NULL)
		return value;
	else
		return XaviTreeNewOperator(OP_POW, value, leftFactor, pool);
}

static XaviTree * GetExpr2lf(XaviLexer * lexer, XaviMemoryPool * pool)
{
	if (XaviLexerPeek(lexer, NULL) == '^')
	{
		XaviLexerNext(lexer);
		return GetExpr2(lexer, pool);
	}
	else
	{
		return NULL;
	}
}

static XaviTree * GetExpr3(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;
	XaviTree * dice;

	value = GetAtom(lexer, pool);
	dice = GetExpr3lf(lexer, pool);

	return (dice == NULL)
		? value
		: XaviTreeNewOperator(OP_DICE, value, dice, pool);
}

static XaviTree * GetExpr3lf(XaviLexer * lexer, XaviMemoryPool * pool)
{
	int token;
	XaviTokenValue value;
	if(XaviLexerPeek(lexer, NULL) == 'd')
	{
		XaviLexerNext(lexer);
		token = XaviLexerPeek(lexer, &value);
		if (token != INTEGER)
			return NULL;
		XaviLexerNext(lexer);
		return XaviTreeNewInteger(value.i, pool);
	}
	else
	{
		return NULL;
	}
}

static XaviTree * GetAtom(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * value;

	switch(XaviLexerPeek(lexer, NULL))
	{
		case '-':
		case INTEGER:
		case FLOAT:
			return GetNumber(lexer, pool);
		case '(':
			XaviLexerNext(lexer);
			value = GetExpr0(lexer, pool);
			if (XaviLexerPeek(lexer, NULL) != ')')
				return NULL;
			XaviLexerNext(lexer);
			return value;
		case ID:
			return GetFCall(lexer, pool);
		default:
			return NULL;
	}
}

static XaviTree * GetNumber(XaviLexer * lexer, XaviMemoryPool * pool)
{
	switch (XaviLexerPeek(lexer, NULL))
	{
		case INTEGER:
		case FLOAT:
			return GetUNumber(lexer, pool);
		case '-':
			XaviLexerNext(lexer);
			XaviTree * Symbol2 = GetUNumber(lexer, pool);
			if (Symbol2 == NULL || !XaviTreeNegate(Symbol2))
				return NULL;

			return Symbol2;
		default:
			return NULL;
	}
}

static XaviTree * GetUNumber(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTokenValue value;

	switch (XaviLexerPeek(lexer, NULL))
	{
		case INTEGER:
			XaviLexerRead(lexer, &value);
			return XaviTreeNewInteger(value.i, pool);
		case FLOAT:
			XaviLexerRead(lexer, &value);
			return XaviTreeNewFloat(value.f, pool);
		default:
			return NULL;
	}
}

static XaviTree * GetFCall(XaviLexer * lexer, XaviMemoryPool * pool)
{
	int token;
	XaviTokenValue value;
	char * id;
	XaviArglist * arglist;

	token = XaviLexerRead(lexer, &value);
	id = XaviFunctionIdNew(value.s, pool);

	token = XaviLexerRead(lexer, &value);
	if (token != '(')
		return NULL;

	arglist = GetArglist(lexer, pool);

	token = XaviLexerRead(lexer, &value);
	if (token != ')')
		return NULL;

	return XaviTreeNewFunction(id, arglist, pool);
}

static XaviArglist * GetArglist(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTree * expression;
	XaviArglist * subArglist;

	expression = GetExpr0(lexer, pool);
	subArglist = GetArglistLF(lexer, pool);

	return XaviArglistNew(expression, subArglist, pool);
}

static XaviArglist * GetArglistLF(XaviLexer * lexer, XaviMemoryPool * pool)
{
	if (XaviLexerPeek(lexer, NULL) != ',')
	{
		return NULL;
	}
	else
	{
		XaviLexerNext(lexer);
		return GetArglist(lexer, pool);
	}
}

int XaviInternalParse(
	XaviNumber * value,
	XaviMemoryPool * pool,
	XaviLexer * lexer)
{
	if (XaviLexerPeek(lexer, NULL) == EOL)
	{
		value->status = S_INTEGER;
		value->i = 0;
		return;
	}

	XaviTree * tree = GetExpr0(lexer, pool);

	if (tree != NULL && XaviLexerPeek(lexer, NULL) == EOL)
	{
		*value = XaviEvaluate(tree);
	}
	else
	{
		value->status = E_SYNTAX;
		value->i = 0;
	}
	XaviCleanupClearAll(pool);
	return 0;
}
