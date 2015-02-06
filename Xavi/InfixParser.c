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

	switch (lexer->token.Type)
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

	switch (lexer->token.Type)
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
	if (lexer->token.Type == '^')
	{
		XaviLexerNext(lexer);

		switch (lexer->token.Type)
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
	int value;

	if(lexer->token.Type == 'd')
	{
		XaviLexerNext(lexer);
		if (lexer->token.Type != INTEGER)
			return XaviTreeNewError();

		value = lexer->token.Integer;
		XaviLexerNext(lexer);
		return XaviTreeNewInteger(value);
	}
	else
	{
		return XaviTreeNewNothing();
	}
}

static XaviTreeNode *GetAtom(XaviLexer *lexer)
{
	XaviTreeNode *value;

	switch(lexer->token.Type)
	{
	case '-':
	case INTEGER:
	case FLOAT:
		return GetNumber(lexer);
	case '(':
		XaviLexerNext(lexer);
		if (!(value = GetExpr0(lexer)))
			return NULL;

		if (lexer->token.Type != ')')
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

	switch (lexer->token.Type)
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
	XaviTreeNode *rVal;

	switch (lexer->token.Type)
	{
	case INTEGER:
		rVal = XaviTreeNewInteger(lexer->token.Integer);
		XaviLexerNext(lexer);
		break;
	case FLOAT:
		rVal = XaviTreeNewFloat(lexer->token.Float);
		XaviLexerNext(lexer);
		break;
	default:
		rVal = XaviTreeNewError();
	}

	return rVal;
}

static XaviTreeNode *GetFCall(XaviLexer *lexer)
{
	char *id;
	XaviTreeNode *rVal;

	if (lexer->token.Type != ID)
		return XaviTreeNewError();

	if (!(id = strdup(lexer->token.String)))
		return NULL;

	XaviLexerNext(lexer);
	if (lexer->token.Type != '(')
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

	if (lexer->token.Type != ')')
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
	switch (lexer->token.Type)
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

XaviTreeNode *XaviParseInfix(XaviDataSource *Input)
{
	XaviTreeNode *rVal = NULL;
	XaviLexer *lexer = NULL;

	if (!(lexer = XaviLexerNew(Input)))
		return NULL;

	rVal = (lexer->token.Type == EOL)
		? XaviTreeNewInteger(0)
		: GetExpr0(lexer);

	if (lexer->token.Type != EOL)
	{
		XaviTreeDelete(rVal);
		rVal = XaviTreeNewError();
	}
	XaviLexerDestroy(lexer);
	return rVal;
}
