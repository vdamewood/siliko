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
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;

	leftValue = GetExpr1(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr0r(lexer)))
	{
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (rest->type == XAVI_NODE_NOTHING)
	{
		XaviTreeDelete(rest);
		return leftValue;
	}

	if (XaviTreeGraftLeft(rest, leftValue))
		return rest;

	XaviTreeDelete(rest);
	XaviTreeDelete(leftValue);
	return XaviTreeNewError();
}

static XaviTreeNode *GetExpr0r(XaviLexer *lexer)
{
	char *operation;
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	XaviTreeNode **operands;
	XaviTreeNode *branchNode;

	switch (XaviLexerGetToken(lexer))
	{
	case '+':
		operation = x_strdup("add");
		break;
	case '-':
		operation = x_strdup("subtract");
		break;
	default:
		return XaviTreeNewNothing();
	}

	if (!operation)
		return NULL;

	XaviLexerNext(lexer);
	if (!(leftValue = GetExpr1(lexer)))
	{
		free(operation);
		return NULL;
	}

	if (!(rest = GetExpr0r(lexer)))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (!(operands = malloc(2 * sizeof(XaviTreeNode *))))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		return NULL;
	}

	operands[0] = NULL;
	operands[1] = leftValue;

	if (!(branchNode = XaviTreeNewBranch(operation, 2, operands)))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		free(operands);
		return NULL;
	}

	if (rest->type != XAVI_NODE_NOTHING)
	{
		XaviTreeGraftLeft(rest, branchNode);
		return rest;
	}

	XaviTreeDelete(rest);
	return branchNode;
}

static XaviTreeNode *GetExpr1(XaviLexer *lexer)
{
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;

	leftValue = GetExpr2(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr1r(lexer)))
	{
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (rest->type == XAVI_NODE_NOTHING)
	{
		XaviTreeDelete(rest);
		return leftValue;
	}

	if (XaviTreeGraftLeft(rest, leftValue))
		return rest;

	XaviTreeDelete(rest);
	XaviTreeDelete(leftValue);
	return XaviTreeNewError();
}

static XaviTreeNode *GetExpr1r(XaviLexer *lexer)
{
	char *operation;
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	XaviTreeNode **operands;
	XaviTreeNode *branchNode;

	switch (XaviLexerGetToken(lexer))
	{
		case '*':
			operation = x_strdup("multiply");
			break;
		case '/':
			operation = x_strdup("divide");
			break;
		default:
			return XaviTreeNewNothing();
	}

	if (!operation)
		return NULL;

	XaviLexerNext(lexer);
	if (!(leftValue = GetExpr2(lexer)))
	{
		free(operation);
		return NULL;
	}

	if (!(rest = GetExpr1r(lexer)))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (!(operands = malloc(2 * sizeof(XaviTreeNode *))))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		return NULL;
	}

	operands[0] = NULL;
	operands[1] = leftValue;

	if (!(branchNode = XaviTreeNewBranch(operation, 2, operands)))
	{
		free(operation);
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		free(operands);
		return NULL;
	}

	if (rest->type != XAVI_NODE_NOTHING)
	{
		XaviTreeGraftLeft(rest, branchNode);
		return rest;
	}

	XaviTreeDelete(rest);
	return branchNode;
}

static XaviTreeNode *GetExpr2(XaviLexer *lexer)
{
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	char *operation;
	XaviTreeNode **operands;

	if (!(leftValue = GetExpr3(lexer)))
		return NULL;

	if (!(rest = GetExpr2lf(lexer)))
	{
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (rest->type == XAVI_NODE_NOTHING)
	{
		XaviTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = x_strdup("power")))
	{
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		return NULL;
	}

	if (!(operands = malloc(2 * sizeof(XaviTreeNode *))))
	{
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		free(operation);
		return NULL;
	}

	operands[0] = leftValue;
	operands[1] = rest;
	return XaviTreeNewBranch(operation, 2, operands);
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
		return XaviTreeNewNothing();
	}
}

static XaviTreeNode *GetExpr3(XaviLexer *lexer)
{
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	char *operation;
	XaviTreeNode **operands;

	if (!(leftValue = GetAtom(lexer)))
		return NULL;

	if (!(rest = GetExpr3lf(lexer)))
	{
		XaviTreeDelete(leftValue);
		return NULL;
	}

	if (rest->type == XAVI_NODE_NOTHING)
	{
		XaviTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = x_strdup("dice")))
	{
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		return NULL;
	}

	if (!(operands = malloc(2 * sizeof(XaviTreeNode *))))
	{
		XaviTreeDelete(leftValue);
		XaviTreeDelete(rest);
		free(operation);
		return NULL;
	}

	operands[0] = leftValue;
	operands[1] = rest;
	return XaviTreeNewBranch(operation, 2, operands);
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
		return XaviTreeNewNothing();
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
		if (!(value = GetExpr0(lexer)))
			return NULL;

		if (XaviLexerGetToken(lexer) != ')')
		{
			XaviTreeDelete(value);
			return XaviTreeNewError();
		}
		XaviLexerNext(lexer);
		return value;
	case ID:
		return GetFCall(lexer);
	default:
		return XaviTreeNewError();
	}
}

static XaviTreeNode *GetNumber(XaviLexer *lexer)
{
	XaviTreeNode * uNumber;

	switch (XaviLexerGetToken(lexer))
	{
	case INTEGER:
	case FLOAT:
		return GetUNumber(lexer);
	case '-':
		XaviLexerNext(lexer);
		if (!(uNumber = GetUNumber(lexer)))
			return NULL;

		if (!XaviTreeNegate(uNumber))
		{
			XaviTreeDelete(uNumber);
			return XaviTreeNewError();
		}

		return uNumber;
	default:
		return XaviTreeNewError();
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
		return XaviTreeNewError();
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

XaviValue XaviParse(XaviLexer *lexer)
{
	XaviValue rVal;
	XaviTreeNode *syntaxTree = NULL;

	if (XaviLexerGetToken(lexer) == EOL)
	{
		rVal.status = XS_INTEGER;
		rVal.i = 0;
	}
	else if (!(syntaxTree = GetExpr0(lexer)))
	{
		rVal.status = XE_MEMORY;
		rVal.i = 0;
	}
	else
	{
		if (syntaxTree->type == XAVI_NODE_ERROR
			|| XaviLexerGetToken(lexer) != EOL)
		{
			rVal.status = XE_SYNTAX;
			rVal.i = 0;
		}
		else
		{
			rVal = XaviTreeEvaluate(syntaxTree);
		}
		XaviTreeDelete(syntaxTree);
	}
	return rVal;
}
