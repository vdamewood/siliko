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

static XaviSyntaxTreeNode *GetExpr0(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr0r(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr1(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr1r(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr2(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr2lf(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetExpr3(XaviLexer *lexe);
static XaviSyntaxTreeNode *GetExpr3lf(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetAtom(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetNumber(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetUNumber(XaviLexer *lexer);
static XaviSyntaxTreeNode *GetFCall(XaviLexer *lexer);
static void GetArguments(XaviLexer *lexer, XaviSyntaxTreeNode *rVal);

static XaviSyntaxTreeNode *GetExpr0(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *leftValue;
	XaviSyntaxTreeNode *rest;

	leftValue = GetExpr1(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr0r(lexer)))
	{
		XaviSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (rest->Type == XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (XaviSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	XaviSyntaxTreeDelete(rest);
	XaviSyntaxTreeDelete(leftValue);
	return XaviSyntaxTreeNewError();
}

static XaviSyntaxTreeNode *GetExpr0r(XaviLexer *lexer)
{
	char *operation = NULL;
	XaviSyntaxTreeNode *leftValue = NULL;
	XaviSyntaxTreeNode *rest = NULL;
	//XaviSyntaxTreeNode **operands = NULL;
	XaviSyntaxTreeNode *branchNode = NULL;

	switch (lexer->Token.Type)
	{
	case '+':
		operation = strdup("add");
		break;
	case '-':
		operation = strdup("subtract");
		break;
	default:
		return XaviSyntaxTreeNewNothing();
	}

	if (!operation)
		goto memerr;

	XaviLexerNext(lexer);
	if (!(leftValue = GetExpr1(lexer)))
		goto memerr;

	if (!(rest = GetExpr0r(lexer)))
		goto memerr;

	//if (!(operands = malloc(2 * sizeof(XaviSyntaxTreeNode *))))
	//	goto memerr;

	//operands[0] = NULL;
	//operands[1] = leftValue;

	if (!(branchNode = XaviSyntaxTreeNewBranch(operation))) //, 2, operands)))
		goto memerr;

	XaviSyntaxTreePushRight(branchNode, NULL);
	XaviSyntaxTreePushRight(branchNode, leftValue);

	if (rest->Type != XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	XaviSyntaxTreeDelete(rest);
	return branchNode;
memerr:
	free(operation);
	XaviSyntaxTreeDelete(leftValue);
	XaviSyntaxTreeDelete(rest);
	//free(operands);
	return NULL;
}

static XaviSyntaxTreeNode *GetExpr1(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *leftValue;
	XaviSyntaxTreeNode *rest;

	leftValue = GetExpr2(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr1r(lexer)))
	{
		XaviSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (rest->Type == XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (XaviSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	XaviSyntaxTreeDelete(rest);
	XaviSyntaxTreeDelete(leftValue);
	return XaviSyntaxTreeNewError();
}

static XaviSyntaxTreeNode *GetExpr1r(XaviLexer *lexer)
{
	char *operation;
	XaviSyntaxTreeNode *leftValue;
	XaviSyntaxTreeNode *rest;
	XaviSyntaxTreeNode *branchNode;

	switch (lexer->Token.Type)
	{
		case '*':
			operation = strdup("multiply");
			break;
		case '/':
			operation = strdup("divide");
			break;
		default:
			return XaviSyntaxTreeNewNothing();
	}

	if (!operation)
		goto memerr;

	XaviLexerNext(lexer);
	if (!(leftValue = GetExpr2(lexer)))
		goto memerr;

	if (!(rest = GetExpr1r(lexer)))
		goto memerr;

	/*if (!(operands = malloc(2 * sizeof(XaviSyntaxTreeNode *))))
	{
		free(operation);
		XaviSyntaxTreeDelete(leftValue);
		XaviSyntaxTreeDelete(rest);
		return NULL;
	}*/

	//operands[0] = NULL;
	//operands[1] = leftValue;

	if (!(branchNode = XaviSyntaxTreeNewBranch(operation))) // , 2, operands)))
		goto memerr;

	XaviSyntaxTreePushRight(branchNode, NULL);
	XaviSyntaxTreePushRight(branchNode, leftValue);

	if (rest->Type != XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	XaviSyntaxTreeDelete(rest);
	return branchNode;
memerr:
	free(operation);
	XaviSyntaxTreeDelete(leftValue);
	XaviSyntaxTreeDelete(rest);
	return NULL;
}

static XaviSyntaxTreeNode *GetExpr2(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *leftValue = NULL;
	XaviSyntaxTreeNode *rest = NULL;
	char *operation = NULL;
	XaviSyntaxTreeNode * rVal;

	if (!(leftValue = GetExpr3(lexer)))
		goto memerr;

	if (!(rest = GetExpr2lf(lexer)))
		goto memerr;

	if (rest->Type == XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = strdup("power")))
		goto memerr;

	if (!(rVal = XaviSyntaxTreeNewBranch(operation)))
		goto memerr;

	XaviSyntaxTreePushRight(rVal, leftValue);
	XaviSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	XaviSyntaxTreeDelete(leftValue);
	XaviSyntaxTreeDelete(rest);
	free(operation);
	return NULL;
}

static XaviSyntaxTreeNode *GetExpr2lf(XaviLexer *lexer)
{
	if (lexer->Token.Type == '^')
	{
		XaviLexerNext(lexer);

		switch (lexer->Token.Type)
		{
		case INTEGER:
		case FLOAT:
		case '-':
		case ID:
		case '(':
			return GetExpr2(lexer);
		default:
			return XaviSyntaxTreeNewError();
		}
	}
	else
	{
		return XaviSyntaxTreeNewNothing();
	}
}

static XaviSyntaxTreeNode *GetExpr3(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *leftValue = NULL;
	XaviSyntaxTreeNode *rest = NULL;
	char *operation = NULL;
	XaviSyntaxTreeNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		goto memerr;

	if (!(rest = GetExpr3lf(lexer)))
		goto memerr;

	if (rest->Type == XAVI_NODE_NOTHING)
	{
		XaviSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = strdup("dice")))
		goto memerr;

	if (!(rVal = XaviSyntaxTreeNewBranch(operation)))
		goto memerr;
	XaviSyntaxTreePushRight(rVal, leftValue);
	XaviSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	XaviSyntaxTreeDelete(leftValue);
	XaviSyntaxTreeDelete(rest);
	free(operation);
	return NULL;

}

static XaviSyntaxTreeNode *GetExpr3lf(XaviLexer *lexer)
{
	int value;

	if(lexer->Token.Type == 'd')
	{
		XaviLexerNext(lexer);
		if (lexer->Token.Type != INTEGER)
			return XaviSyntaxTreeNewError();

		value = lexer->Token.Integer;
		XaviLexerNext(lexer);
		return XaviSyntaxTreeNewInteger(value);
	}
	else
	{
		return XaviSyntaxTreeNewNothing();
	}
}

static XaviSyntaxTreeNode *GetAtom(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *value;

	switch(lexer->Token.Type)
	{
	case '-':
	case INTEGER:
	case FLOAT:
		return GetNumber(lexer);
	case '(':
		XaviLexerNext(lexer);
		if (!(value = GetExpr0(lexer)))
			return NULL;

		if (lexer->Token.Type != ')')
		{
			XaviSyntaxTreeDelete(value);
			return XaviSyntaxTreeNewError();
		}
		XaviLexerNext(lexer);
		return value;
	case ID:
		return GetFCall(lexer);
	default:
		return XaviSyntaxTreeNewError();
	}
}

static XaviSyntaxTreeNode *GetNumber(XaviLexer *lexer)
{
	XaviSyntaxTreeNode * uNumber;

	switch (lexer->Token.Type)
	{
	case INTEGER:
	case FLOAT:
		return GetUNumber(lexer);
	case '-':
		XaviLexerNext(lexer);
		if (!(uNumber = GetUNumber(lexer)))
			return NULL;

		if (!XaviSyntaxTreeNegate(uNumber))
		{
			XaviSyntaxTreeDelete(uNumber);
			return XaviSyntaxTreeNewError();
		}

		return uNumber;
	default:
		return XaviSyntaxTreeNewError();
	}
}

static XaviSyntaxTreeNode *GetUNumber(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *rVal;

	switch (lexer->Token.Type)
	{
	case INTEGER:
		rVal = XaviSyntaxTreeNewInteger(lexer->Token.Integer);
		XaviLexerNext(lexer);
		break;
	case FLOAT:
		rVal = XaviSyntaxTreeNewFloat(lexer->Token.Float);
		XaviLexerNext(lexer);
		break;
	default:
		rVal = XaviSyntaxTreeNewError();
	}

	return rVal;
}

/*static XaviSyntaxTreeNode *GetFCall(XaviLexer *lexer)
{
	char *id;
	XaviSyntaxTreeNode *rVal;

	if (lexer->Token.Type != ID)
		return XaviSyntaxTreeNewError();

	if (!(id = strdup(lexer->Token.String)))
		return NULL;

	XaviLexerNext(lexer);
	if (lexer->Token.Type != '(')
	{
		free(id);
		return XaviSyntaxTreeNewError();
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
			if (!XaviSyntaxTreeCollapseBranch(rVal))
			{
				XaviSyntaxTreeDelete(rVal);
				return NULL;
			}
			break;
		case XAVI_NODE_ERROR:
			free(id);
			return rVal;
		default:
			free(id);
			XaviSyntaxTreeDelete(rVal);
			return XaviSyntaxTreeNewError();
	}

	if (lexer->Token.Type != ')')
	{
		XaviSyntaxTreeDelete(rVal);
		return XaviSyntaxTreeNewError();
	}
	XaviLexerNext(lexer);

	return rVal;
}

static XaviSyntaxTreeNode *GetArguments(XaviLexer *lexer)
{
	XaviSyntaxTreeNode *expression;
	XaviSyntaxTreeNode *rest;

	expression = GetExpr0(lexer);

	if (expression == NULL)
		return NULL;

	rest = GetNextArgument(lexer);

	if (rest == NULL)
	{
		XaviSyntaxTreeDelete(expression);
		return NULL;
	}

	switch (rest->type)
	{
	case XAVI_NODE_LIST_BRANCH:
		if (!XaviSyntaxTreePushFront(rest, expression))
		{
			XaviSyntaxTreeDelete(expression);
			XaviSyntaxTreeDelete(rest);
			return NULL;
		}
		break;
	case XAVI_NODE_ERROR:
			XaviSyntaxTreeDelete(expression);
		break;
	case XAVI_NODE_NOTHING:
		free(rest);
		if (!(rest = XaviSyntaxTreeNewListBranch(expression)))
			free(expression);
		break;
	default:
		XaviSyntaxTreeDelete(expression);
		XaviSyntaxTreeDelete(rest);
		rest = XaviSyntaxTreeNewError();
	}

	return rest;
}

static XaviSyntaxTreeNode *GetNextArgument(XaviLexer *lexer)
{
	switch (lexer->Token.Type)
	{
	case ',':
		XaviLexerNext(lexer);
		return GetArguments(lexer);
	case ')':
		return XaviSyntaxTreeNewNothing();
	default:
		return XaviSyntaxTreeNewError();
	}
}*/

XaviSyntaxTreeNode *GetFCall(XaviLexer *lexer)
{
	if (lexer->Token.Type != ID)
		return XaviSyntaxTreeNewError();

	XaviSyntaxTreeNode *rVal = XaviSyntaxTreeNewBranch(lexer->Token.Id);
	XaviLexerNext(lexer);

	if (lexer->Token.Type != '(')
	{
		XaviSyntaxTreePushRight(rVal, XaviSyntaxTreeNewError());
		return rVal;
	}
	XaviLexerNext(lexer);

	GetArguments(lexer, rVal);

	if (lexer->Token.Type != ')')
	{
		XaviSyntaxTreePushRight(rVal, XaviSyntaxTreeNewError());
		return rVal;
	}
	XaviLexerNext(lexer);

	return rVal;
}

static void GetArguments(XaviLexer *lexer, XaviSyntaxTreeNode *rVal)
{
	while(-1)
	{
		XaviSyntaxTreeNode *Expression = GetExpr0(lexer);
		XaviSyntaxTreePushRight(rVal, Expression);

		if (Expression->Type == XAVI_NODE_ERROR
			|| lexer->Token.Type == ')')
		{
			break;
		}
		else if (lexer->Token.Type != ',')
		{
			XaviSyntaxTreePushRight(rVal, XaviSyntaxTreeNewError());
			break;
		}
		XaviLexerNext(lexer);
	}
}


XaviSyntaxTreeNode *XaviParseInfix(XaviDataSource *Input)
{
	XaviSyntaxTreeNode *rVal = NULL;
	XaviLexer *lexer = NULL;

	if (!(lexer = XaviLexerNew(Input)))
		return NULL;

	rVal = (lexer->Token.Type == EOL)
		? XaviSyntaxTreeNewInteger(0)
		: GetExpr0(lexer);

	if (lexer->Token.Type != EOL)
	{
		XaviSyntaxTreeDelete(rVal);
		rVal = XaviSyntaxTreeNewError();
	}
	XaviLexerDelete(lexer);
	return rVal;
}
