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
#include <string.h>

#include "XaviArglist.h"
#include "XaviParser.h"
#include "XaviTree.h"

static char *x_strdup(const char *string)
{
	char *rVal;

	if (rVal = malloc(strlen(string) + 1))
		strcpy(rVal, string);

	return rVal;
}

static XaviTreeNode *GetExpr0(XaviLexer *lexer);
static XaviTreeNode *GetExpr0r(XaviLexer *lexer);
static XaviTreeNode *GetExpr1(XaviLexer *lexer);
static XaviTreeNode *GetExpr1r(XaviLexer *lexer);
static XaviTreeNode *GetExpr2(XaviLexer *lexer);
static XaviTreeNode *GetExpr2lf(XaviLexer *lexer);
static XaviTreeNode *GetExpr3(XaviLexer *lexe);
static XaviTreeNode *GetExpr3lf(XaviLexer *lexer);
static XaviTreeNode *GetAtom(XaviLexer *lexer);
static XaviTreeNode *GetNumber(XaviLexer *lexer);
static XaviTreeNode *GetUNumber(XaviLexer *lexer);
static XaviTreeNode *GetFCall(XaviLexer *lexer);
static XaviArglist *GetArglist(XaviLexer *lexer);
static XaviArglist *GetArglistLF(XaviLexer *lexer);

static XaviTreeNode *GetExpr0(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *rest;

	value = GetExpr1(lexer);

	if (value == NULL)
		return NULL;
	rest = GetExpr0r(lexer);

	if (rest == NULL)
	{
		return value;
	}
	else
	{
		XaviTreeGraftLeft(rest, value);
		return rest;
	}
}

static XaviTreeNode *GetExpr0r(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *rest;
	char *function;
	XaviTreeNode *functionCall;
	XaviTreeNode **arguments;

	switch (XaviLexerGetToken(lexer))
	{
	case '+':
		function = x_strdup("add");
		break;
	case '-':
		function = x_strdup("subtract");
		break;
	default:
		return NULL;
	}

	XaviLexerNext(lexer);
	value = GetExpr1(lexer);

	rest = GetExpr0r(lexer);

	arguments = malloc(2 * sizeof(XaviTreeNode *));

	arguments[0] = NULL;
	arguments[1] = value;

	functionCall = XaviTreeNewBranch(function, 2, arguments);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(rest, functionCall);
		return rest;
	}
	else
	{
		return functionCall;
	}
}

static XaviTreeNode *GetExpr1(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *rest;

	value = GetExpr2(lexer);
	if (value == NULL)
		return NULL;
	rest = GetExpr1r(lexer);

	if (rest == NULL)
	{
		return value;
	}
	else
	{
		XaviTreeGraftLeft(rest, value);
		return rest;
	}
}

static XaviTreeNode *GetExpr1r(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *rest;
	char *function;
	XaviTreeNode *functionCall;
	XaviTreeNode **arguments;

	switch (XaviLexerGetToken(lexer))
	{
	case '*':
		function = x_strdup("multiply");
		break;
	case '/':
		function = x_strdup("divide");
		break;
	default:
		return NULL;
	}

	XaviLexerNext(lexer);
	value = GetExpr2(lexer);

	rest = GetExpr1r(lexer);

	arguments = malloc(2 * sizeof(XaviTreeNode *));

	arguments[0] = NULL;
	arguments[1] = value;

	functionCall = XaviTreeNewBranch(function, 2, arguments);

	if (rest != NULL)
	{
		XaviTreeGraftLeft(rest, functionCall);
		return rest;
	}
	else
	{
		return functionCall;
	}
}

static XaviTreeNode *GetExpr2(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *leftFactor;
	char *functionName;
	XaviTreeNode **arguments;

	value = GetExpr3(lexer);
	if (value == NULL)
		return NULL;
	leftFactor = GetExpr2lf(lexer);

	if (leftFactor == NULL)
	{
		return value;
	}
	else
	{
		functionName = x_strdup("power");
		arguments = malloc(2 * sizeof(XaviTreeNode *));
		arguments[0] = value;
		arguments[1] = leftFactor;
		return XaviTreeNewBranch(functionName, 2, arguments);
	}
}

static XaviTreeNode *GetExpr2lf(XaviLexer *lexer)
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
			return XaviTreeNewError();
		}
	}
	else
	{
		return NULL;
	}
}

static XaviTreeNode * GetExpr3(XaviLexer *lexer)
{
	XaviTreeNode *value;
	XaviTreeNode *dice;
	char *functionName;
	XaviTreeNode **arguments;

	value = GetAtom(lexer);
	dice = GetExpr3lf(lexer);

	if (dice == NULL)
	{
		return value;
	}
	else
	{
		functionName = x_strdup("dice");
		arguments = malloc(2 * sizeof(XaviTreeNode *));
		arguments[0] = value;
		arguments[1] = dice;
		return XaviTreeNewBranch(functionName, 2, arguments);
	}
}

static XaviTreeNode *GetExpr3lf(XaviLexer *lexer)
{
	XaviTokenValue value;

	if(XaviLexerGetToken(lexer) == 'd')
	{
		XaviLexerNext(lexer);
		if (XaviLexerGetToken(lexer) != INTEGER)
			return XaviTreeNewError();

		value = XaviLexerGetValue(lexer);
		XaviLexerNext(lexer);
		return XaviTreeNewInteger(value.i);
	}
	else
	{
		return NULL;
	}
}

static XaviTreeNode *GetAtom(XaviLexer *lexer)
{
	XaviTreeNode *value;

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

static XaviTreeNode *GetNumber(XaviLexer *lexer)
{
	switch (XaviLexerGetToken(lexer))
	{
	case INTEGER:
	case FLOAT:
		return GetUNumber(lexer);
	case '-':
		XaviLexerNext(lexer);
		XaviTreeNode *Symbol2 = GetUNumber(lexer);
		if (Symbol2 == NULL || !XaviTreeNegate(Symbol2))
			return NULL;
		return Symbol2;
	default:
		return NULL;
	}
}

static XaviTreeNode *GetUNumber(XaviLexer *lexer)
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

static XaviTreeNode *GetFCall(XaviLexer *lexer)
{
	int token;
	XaviTokenValue value;
	char *id;
	int collapsedArgc;
	XaviTreeNode **collapsedArgv;
	XaviArglist *arglist;

	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);

	char *rVal;
	id = malloc(strlen(value.s) + 1);
	strcpy(id, value.s);

	XaviLexerNext(lexer);
	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);

	XaviLexerNext(lexer);
	if (token != '(')
	{
		free(id);
		return NULL;
	}

	arglist = GetArglist(lexer);

	token = XaviLexerGetToken(lexer);
	value = XaviLexerGetValue(lexer);
	XaviLexerNext(lexer);

	if (token != ')')
	{
		free(id);
		XaviArglistDelete(arglist);
		return NULL;
	}

	if(arglist)
	{
		collapsedArgc = arglist->depth;
		collapsedArgv = XaviArglistGetTrees(arglist);
	}
	else
	{
		collapsedArgc = 0;
		collapsedArgv = NULL;
	}

	return XaviTreeNewBranch(id, collapsedArgc, collapsedArgv);
}

static XaviArglist *GetArglist(XaviLexer *lexer)
{
	XaviTreeNode *expression;
	XaviArglist *subArglist;

	expression = GetExpr0(lexer);
	subArglist = GetArglistLF(lexer);

	return XaviArglistNew(expression, subArglist);
}

static XaviArglist *GetArglistLF(XaviLexer *lexer)
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

XaviValue XaviInternalParse(XaviLexer *lexer)
{
	XaviValue value;

	if (XaviLexerGetToken(lexer) == EOL)
	{
		value.status = XS_INTEGER;
		value.i = 0;
		return value;
	}

	XaviTreeNode *tree = GetExpr0(lexer);

	if (tree != NULL && tree->type != 'e' && XaviLexerGetToken(lexer) == EOL)
	{
		value = XaviTreeEvaluate(tree);
	}
	else
	{
		value.status = XE_SYNTAX;
		value.i = 0;
	}
	XaviTreeDelete(tree);

	return value;
}
