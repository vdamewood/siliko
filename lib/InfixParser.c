/* InfixParser.c: Infix notation parser
 * Copyright 2014, 2015, 2016 Vincent Damewood
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

#if defined _WIN32
#define strdup _strdup
#endif

static SilikoSyntaxTreeNode *GetExpr0(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr0r(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr1(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr1r(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr2(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr2lf(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpr3(SilikoLexer *lexe);
static SilikoSyntaxTreeNode *GetExpr3lf(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetUNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetFCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal);

static SilikoSyntaxTreeNode *GetExpr0(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue;
	SilikoSyntaxTreeNode *rest;

	leftValue = GetExpr1(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr0r(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (SilikoSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	SilikoSyntaxTreeDelete(rest);
	SilikoSyntaxTreeDelete(leftValue);
	return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetExpr0r(SilikoLexer *lexer)
{
	char *operation = NULL;
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *branchNode = NULL;

	switch (lexer->Token.Type)
	{
	case '+':
		operation = strdup("add");
		break;
	case '-':
		operation = strdup("subtract");
		break;
	default:
		return SilikoSyntaxTreeNewNothing();
	}

	if (!operation)
		goto memerr;

	SilikoLexerNext(lexer);
	if (!(leftValue = GetExpr1(lexer)))
		goto memerr;

	if (!(rest = GetExpr0r(lexer)))
		goto memerr;

	if (!(branchNode = SilikoSyntaxTreeNewBranch(operation)))
		goto memerr;

	SilikoSyntaxTreePushRight(branchNode, NULL);
	SilikoSyntaxTreePushRight(branchNode, leftValue);

	if (rest->Type != SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	SilikoSyntaxTreeDelete(rest);
	return branchNode;
memerr:
	free(operation);
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExpr1(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue;
	SilikoSyntaxTreeNode *rest;

	leftValue = GetExpr2(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExpr1r(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (SilikoSyntaxTreeGraftLeft(rest, leftValue))
		return rest;

	SilikoSyntaxTreeDelete(rest);
	SilikoSyntaxTreeDelete(leftValue);
	return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetExpr1r(SilikoLexer *lexer)
{
	char *operation;
	SilikoSyntaxTreeNode *leftValue;
	SilikoSyntaxTreeNode *rest;
	SilikoSyntaxTreeNode *branchNode;

	switch (lexer->Token.Type)
	{
		case '*':
			operation = strdup("multiply");
			break;
		case '/':
			operation = strdup("divide");
			break;
		default:
			return SilikoSyntaxTreeNewNothing();
	}

	if (!operation)
		goto memerr;

	SilikoLexerNext(lexer);
	if (!(leftValue = GetExpr2(lexer)))
		goto memerr;

	if (!(rest = GetExpr1r(lexer)))
		goto memerr;

	if (!(branchNode = SilikoSyntaxTreeNewBranch(operation)))
		goto memerr;

	SilikoSyntaxTreePushRight(branchNode, NULL);
	SilikoSyntaxTreePushRight(branchNode, leftValue);

	if (rest->Type != SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeGraftLeft(rest, branchNode);
		return rest;
	}

	SilikoSyntaxTreeDelete(rest);
	return branchNode;
memerr:
	free(operation);
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExpr2(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	char *operation = NULL;
	SilikoSyntaxTreeNode * rVal;

	if (!(leftValue = GetExpr3(lexer)))
		goto memerr;

	if (!(rest = GetExpr2lf(lexer)))
		goto memerr;

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = strdup("power")))
		goto memerr;

	if (!(rVal = SilikoSyntaxTreeNewBranch(operation)))
		goto memerr;

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	free(operation);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExpr2lf(SilikoLexer *lexer)
{
	if (lexer->Token.Type == '^')
	{
		SilikoLexerNext(lexer);

		switch (lexer->Token.Type)
		{
		case SILIKO_TOK_INTEGER:
		case SILIKO_TOK_FLOAT:
		case '-':
		case SILIKO_TOK_ID:
		case '(':
			return GetExpr2(lexer);
		default:
			return SilikoSyntaxTreeNewError();
		}
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetExpr3(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	char *operation = NULL;
	SilikoSyntaxTreeNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		goto memerr;

	if (!(rest = GetExpr3lf(lexer)))
		goto memerr;

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(operation = strdup("dice")))
		goto memerr;

	if (!(rVal = SilikoSyntaxTreeNewBranch(operation)))
		goto memerr;
	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	free(operation);
	return NULL;

}

static SilikoSyntaxTreeNode *GetExpr3lf(SilikoLexer *lexer)
{
	int value;

	if(lexer->Token.Type == 'd')
	{
		SilikoLexerNext(lexer);
		if (lexer->Token.Type != SILIKO_TOK_INTEGER)
			return SilikoSyntaxTreeNewError();

		value = lexer->Token.Integer;
		SilikoLexerNext(lexer);
		return SilikoSyntaxTreeNewInteger(value);
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *value;

	switch(lexer->Token.Type)
	{
	case '-':
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetNumber(lexer);
	case '(':
		SilikoLexerNext(lexer);
		if (!(value = GetExpr0(lexer)))
			return NULL;

		if (lexer->Token.Type != ')')
		{
			SilikoSyntaxTreeDelete(value);
			return SilikoSyntaxTreeNewError();
		}
		SilikoLexerNext(lexer);
		return value;
	case SILIKO_TOK_ID:
		return GetFCall(lexer);
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode * uNumber;

	switch (lexer->Token.Type)
	{
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetUNumber(lexer);
	case '-':
		SilikoLexerNext(lexer);
		if (!(uNumber = GetUNumber(lexer)))
			return NULL;

		if (!SilikoSyntaxTreeNegate(uNumber))
		{
			SilikoSyntaxTreeDelete(uNumber);
			return SilikoSyntaxTreeNewError();
		}

		return uNumber;
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetUNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal;

	switch (lexer->Token.Type)
	{
	case SILIKO_TOK_INTEGER:
		rVal = SilikoSyntaxTreeNewInteger(lexer->Token.Integer);
		SilikoLexerNext(lexer);
		break;
	case SILIKO_TOK_FLOAT:
		rVal = SilikoSyntaxTreeNewFloat(lexer->Token.Float);
		SilikoLexerNext(lexer);
		break;
	default:
		rVal = SilikoSyntaxTreeNewError();
	}

	return rVal;
}

SilikoSyntaxTreeNode *GetFCall(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if (lexer->Token.Type != SILIKO_TOK_ID)
		return SilikoSyntaxTreeNewError();

	rVal = SilikoSyntaxTreeNewBranch(lexer->Token.Id);
	SilikoLexerNext(lexer);

	if (lexer->Token.Type != '(')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerNext(lexer);

	GetArguments(lexer, rVal);

	if (lexer->Token.Type != ')')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerNext(lexer);

	return rVal;
}

static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal)
{
	while(-1)
	{
		SilikoSyntaxTreeNode *Expression = GetExpr0(lexer);
		SilikoSyntaxTreePushRight(rVal, Expression);

		if (Expression->Type == SILIKO_AST_ERROR
			|| lexer->Token.Type == ')')
		{
			break;
		}
		else if (lexer->Token.Type != ',')
		{
			SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
			break;
		}
		SilikoLexerNext(lexer);
	}
}


SilikoSyntaxTreeNode *SilikoParseInfix(SilikoDataSource *Input)
{
	SilikoSyntaxTreeNode *rVal = NULL;
	SilikoLexer *lexer = NULL;

	if (!(lexer = SilikoLexerNew(Input)))
		return NULL;

	rVal = (lexer->Token.Type == SILIKO_TOK_EOL)
		? SilikoSyntaxTreeNewInteger(0)
		: GetExpr0(lexer);

	if (lexer->Token.Type != SILIKO_TOK_EOL)
	{
		SilikoSyntaxTreeDelete(rVal);
		rVal = SilikoSyntaxTreeNewError();
	}
	SilikoLexerDelete(lexer);
	return rVal;
}
