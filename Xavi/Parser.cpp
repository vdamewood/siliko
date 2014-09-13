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

#include "Parser.hpp"
#include "SyntaxTree.hpp"

static char *x_strdup(const char *string)
{
	char *rVal;

	if ((rVal = (char*) malloc(strlen(string) + 1)))
		strcpy(rVal, string);

	return rVal;
}

static XaviTreeNode *GetExpr0(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr0r(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr1(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr1r(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr2(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr2lf(Xavi::Lexer *lexer);
static XaviTreeNode *GetExpr3(Xavi::Lexer *lexe);
static XaviTreeNode *GetExpr3lf(Xavi::Lexer *lexer);
static XaviTreeNode *GetAtom(Xavi::Lexer *lexer);
static XaviTreeNode *GetNumber(Xavi::Lexer *lexer);
static XaviTreeNode *GetUNumber(Xavi::Lexer *lexer);
static XaviTreeNode *GetFCall(Xavi::Lexer *lexer);
static XaviTreeNode *GetArguments(Xavi::Lexer *lexer);
static XaviTreeNode *GetNextArgument(Xavi::Lexer *lexer);

static XaviTreeNode *GetExpr0(Xavi::Lexer *lexer)
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

static XaviTreeNode *GetExpr0r(Xavi::Lexer *lexer)
{
	char *operation;
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	XaviTreeNode **operands;
	XaviTreeNode *branchNode;

	switch (lexer->GetToken())
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

	lexer->Next();
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

	if (!(operands = (XaviTreeNode**) malloc(2 * sizeof(XaviTreeNode *))))
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

static XaviTreeNode *GetExpr1(Xavi::Lexer *lexer)
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

static XaviTreeNode *GetExpr1r(Xavi::Lexer *lexer)
{
	char *operation;
	XaviTreeNode *leftValue;
	XaviTreeNode *rest;
	XaviTreeNode **operands;
	XaviTreeNode *branchNode;

	switch (lexer->GetToken())
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

	lexer->Next();
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

	if (!(operands = (XaviTreeNode**) malloc(2 * sizeof(XaviTreeNode *))))
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

static XaviTreeNode *GetExpr2(Xavi::Lexer *lexer)
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

	if (!(operands = (XaviTreeNode**) malloc(2 * sizeof(XaviTreeNode *))))
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

static XaviTreeNode *GetExpr2lf(Xavi::Lexer *lexer)
{
	if (lexer->GetToken() == '^')
	{
		lexer->Next();

		switch (lexer->GetToken())
		{
		case Xavi::INTEGER:
		case Xavi::FLOAT:
		case '-':
		case Xavi::ID:
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

static XaviTreeNode *GetExpr3(Xavi::Lexer *lexer)
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

	if (!(operands = (XaviTreeNode**) malloc(2 * sizeof(XaviTreeNode *))))
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

static XaviTreeNode *GetExpr3lf(Xavi::Lexer *lexer)
{
	Xavi::TokenValue value;

	if(lexer->GetToken() == 'd')
	{
		lexer->Next();
		if (lexer->GetToken() != Xavi::INTEGER)
			return XaviTreeNewError();

		value = lexer->GetValue();
		lexer->Next();
		return XaviTreeNewInteger(value.i);
	}
	else
	{
		return XaviTreeNewNothing();
	}
}

static XaviTreeNode *GetAtom(Xavi::Lexer *lexer)
{
	XaviTreeNode *value;

	switch(lexer->GetToken())
	{
	case '-':
	case Xavi::INTEGER:
	case Xavi::FLOAT:
		return GetNumber(lexer);
	case '(':
		lexer->Next();
		if (!(value = GetExpr0(lexer)))
			return NULL;

		if (lexer->GetToken() != ')')
		{
			XaviTreeDelete(value);
			return XaviTreeNewError();
		}
		lexer->Next();
		return value;
	case Xavi::ID:
		return GetFCall(lexer);
	default:
		return XaviTreeNewError();
	}
}

static XaviTreeNode *GetNumber(Xavi::Lexer *lexer)
{
	XaviTreeNode * uNumber;

	switch (lexer->GetToken())
	{
	case Xavi::INTEGER:
	case Xavi::FLOAT:
		return GetUNumber(lexer);
	case '-':
		lexer->Next();
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

static XaviTreeNode *GetUNumber(Xavi::Lexer *lexer)
{
	Xavi::TokenValue value;

	switch (lexer->GetToken())
	{
	case Xavi::INTEGER:
		value = lexer->GetValue();
		lexer->Next();
		return XaviTreeNewInteger(value.i);
	case Xavi::FLOAT:
		value = lexer->GetValue();
		lexer->Next();
		return XaviTreeNewFloat(value.f);
	default:
		return XaviTreeNewError();
	}
}

static XaviTreeNode *GetFCall(Xavi::Lexer *lexer)
{
	char *id;
	XaviTreeNode *rVal;

	if (lexer->GetToken() != Xavi::ID)
		return XaviTreeNewError();

	if (!(id = x_strdup(lexer->GetValue().s)))
		return NULL;

	lexer->Next();
	if (lexer->GetToken() != '(')
	{
		free(id);
		return XaviTreeNewError();
	}

	lexer->Next();
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

	if (lexer->GetToken() != ')')
	{
		XaviTreeDelete(rVal);
		return XaviTreeNewError();
	}
	lexer->Next();

	return rVal;
}

static XaviTreeNode *GetArguments(Xavi::Lexer *lexer)
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

static XaviTreeNode *GetNextArgument(Xavi::Lexer *lexer)
{
	switch (lexer->GetToken())
	{
	case ',':
		lexer->Next();
		return GetArguments(lexer);
	case ')':
		return XaviTreeNewNothing();
	default:
		return XaviTreeNewError();
	}
}

XaviValue Xavi::Parse(Xavi::Lexer *lexer)
{
	XaviValue rVal;
	XaviTreeNode *syntaxTree = NULL;

	if (lexer->GetToken() == EOL)
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
			|| lexer->GetToken() != EOL)
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
