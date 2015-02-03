/*
 * XaviParser.c: Parser for Xavi
 * Copyright 2014, 2015 Vincent Damewood
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

#include <stdlib.h>
#include <string.h>

#include "InfixParser.h"
#include "SyntaxTree.h"
#include "Lexer.h"

#if defined WIN32
#define strdup _strdup
#endif

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
static XaviTreeNode *GetArguments(XaviLexer *lexer);
static XaviTreeNode *GetNextArgument(XaviLexer *lexer);

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
		operation = strdup("add");
		break;
	case '-':
		operation = strdup("subtract");
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

	if (!(branchNode = XaviTreeNewVectorBranch(operation, 2, operands)))
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
			operation = strdup("multiply");
			break;
		case '/':
			operation = strdup("divide");
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

	if (!(branchNode = XaviTreeNewVectorBranch(operation, 2, operands)))
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

	if (!(operation = strdup("power")))
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
	return XaviTreeNewVectorBranch(operation, 2, operands);
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

	if (!(operation = strdup("dice")))
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
	return XaviTreeNewVectorBranch(operation, 2, operands);
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
	char *id;
	XaviTreeNode *rVal;

	if (XaviLexerGetToken(lexer) != ID)
		return XaviTreeNewError();

	if (!(id = strdup(XaviLexerGetValue(lexer).s)))
		return NULL;

	XaviLexerNext(lexer);
	if (XaviLexerGetToken(lexer) != '(')
	{
		free(id);
		return XaviTreeNewError();
	}

	XaviLexerNext(lexer);
	if (!(rVal = GetArguments(lexer)))
	{
		free(id);
		return NULL;
	}

	switch (rVal->type)
	{
		case XAVI_NODE_LIST_BRANCH:
			rVal->branch->id = id;
			if (!XaviTreeCollapseBranch(rVal))
			{
				XaviTreeDelete(rVal);
				return NULL;
			}
			break;
		case XAVI_NODE_ERROR:
			free(id);
			return rVal;
		default:
			free(id);
			XaviTreeDelete(rVal);
			return XaviTreeNewError();
	}

	if (XaviLexerGetToken(lexer) != ')')
	{
		XaviTreeDelete(rVal);
		return XaviTreeNewError();
	}
	XaviLexerNext(lexer);

	return rVal;
}

static XaviTreeNode *GetArguments(XaviLexer *lexer)
{
	XaviTreeNode *expression;
	XaviTreeNode *rest;

	expression = GetExpr0(lexer);

	if (expression == NULL)
		return NULL;

	rest = GetNextArgument(lexer);

	if (rest == NULL)
	{
		XaviTreeDelete(expression);
		return NULL;
	}

	switch (rest->type)
	{
	case XAVI_NODE_LIST_BRANCH:
		if (!XaviTreePushFront(rest, expression))
		{
			XaviTreeDelete(expression);
			XaviTreeDelete(rest);
			return NULL;
		}
		break;
	case XAVI_NODE_ERROR:
			XaviTreeDelete(expression);
		break;
	case XAVI_NODE_NOTHING:
		free(rest);
		if (!(rest = XaviTreeNewListBranch(expression)))
			free(expression);
		break;
	default:
		XaviTreeDelete(expression);
		XaviTreeDelete(rest);
		rest = XaviTreeNewError();
	}

	return rest;
}

static XaviTreeNode *GetNextArgument(XaviLexer *lexer)
{
	switch (XaviLexerGetToken(lexer))
	{
	case ',':
		XaviLexerNext(lexer);
		return GetArguments(lexer);
	case ')':
		return XaviTreeNewNothing();
	default:
		return XaviTreeNewError();
	}
}

XaviValue XaviParse(XaviDataSource *Input)
{
	XaviValue rVal;
	XaviTreeNode *syntaxTree = NULL;
	XaviLexer *lexer = NULL;

	if (!(lexer = XaviLexerNew(Input)))
		goto memerr;


	if (XaviLexerGetToken(lexer) == EOL)
	{
		rVal.status = XAVI_INTEGER;
		rVal.i = 0;
	}
	else if (!(syntaxTree = GetExpr0(lexer)))
	{
		goto memerr;
	}
	else
	{
		if (syntaxTree->type == XAVI_NODE_ERROR
			|| XaviLexerGetToken(lexer) != EOL)
		{
			rVal.status = XAVI_SYNTAX_ERR;
			rVal.i = 0;
		}
		else
		{
			rVal = XaviTreeEvaluate(syntaxTree);
		}
		XaviTreeDelete(syntaxTree);
	}

	XaviLexerDestroy(lexer);
	return rVal;
memerr:
	XaviTreeDelete(syntaxTree);
	XaviLexerDestroy(lexer);

	rVal.status = XAVI_MEMORY_ERR;
	rVal.i = 0;
	return rVal;
}
