/* InfixParser.c: Infix notation parser
 * Copyright 2012-2021 Vincent Damewood
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

#include <Siliko/InfixParser.h>
#include <Siliko/SyntaxTree.h>
#include <Siliko/Lexer.h>

static SilikoSyntaxTreeNode *GetExprAddSub(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprAddSubRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprMulDiv(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprMulDivRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprExp(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprExpLeftFactor(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExprRoll(SilikoLexer *lexe);
static SilikoSyntaxTreeNode *GetExprRollLeftFactor(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetUNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetFCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal);

static SilikoSyntaxTreeNode *GetExprAddSub(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;

	leftValue = GetExprMulDiv(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExprAddSubRest(lexer)))
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

static SilikoSyntaxTreeNode *GetExprAddSubRest(SilikoLexer *lexer)
{
	char *operation = NULL;
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *branchNode = NULL;

	switch (lexer->Token.Type)
	{
	case '+':
		operation = "add";
		break;
	case '-':
		operation = "subtract";
		break;
	default:
		return SilikoSyntaxTreeNewNothing();
	}

	SilikoLexerNext(lexer);
	if (!(leftValue = GetExprMulDiv(lexer)))
		goto memerr;

	if (!(rest = GetExprAddSubRest(lexer)))
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
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExprMulDiv(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;

	leftValue = GetExprExp(lexer);

	if (leftValue == NULL)
		return NULL;

	if (!(rest = GetExprMulDivRest(lexer)))
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

static SilikoSyntaxTreeNode *GetExprMulDivRest(SilikoLexer *lexer)
{
	char *operation;
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *branchNode = NULL;

	switch (lexer->Token.Type)
	{
		case '*':
			operation = "multiply";
			break;
		case '/':
			operation = "divide";
			break;
		default:
			return SilikoSyntaxTreeNewNothing();
	}

	SilikoLexerNext(lexer);
	if (!(leftValue = GetExprExp(lexer)))
		goto memerr;

	if (!(rest = GetExprMulDivRest(lexer)))
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
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExprExp(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode * rVal = NULL;

	if (!(leftValue = GetExprRoll(lexer)))
		goto memerr;

	if (!(rest = GetExprExpLeftFactor(lexer)))
		goto memerr;

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("power")))
		goto memerr;

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;
}

static SilikoSyntaxTreeNode *GetExprExpLeftFactor(SilikoLexer *lexer)
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
			return GetExprExp(lexer);
		default:
			return SilikoSyntaxTreeNewError();
		}
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetExprRoll(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		goto memerr;

	if (!(rest = GetExprRollLeftFactor(lexer)))
		goto memerr;

	if (rest->Type == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("dice")))
		goto memerr;
	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
memerr:
	SilikoSyntaxTreeDelete(leftValue);
	SilikoSyntaxTreeDelete(rest);
	return NULL;

}

static SilikoSyntaxTreeNode *GetExprRollLeftFactor(SilikoLexer *lexer)
{
	long long int value;

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
		if (!(value = GetExprAddSub(lexer)))
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
	SilikoSyntaxTreeNode * uNumber = NULL;

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
	SilikoSyntaxTreeNode *rVal = NULL;

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
		SilikoSyntaxTreeNode *Expression = GetExprAddSub(lexer);
		SilikoSyntaxTreePushRight(rVal, Expression);

		if (
			(Expression->Type == SILIKO_AST_LEAF
				&& Expression->Leaf.Status == SILIKO_VAL_SYNTAX_ERR)
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
		: GetExprAddSub(lexer);

	if (lexer->Token.Type != SILIKO_TOK_EOL)
	{
		SilikoSyntaxTreeDelete(rVal);
		rVal = SilikoSyntaxTreeNewError();
	}
	SilikoLexerDelete(lexer);
	return rVal;
}
