/* InfixParser.c: Infix notation parser
 * Copyright 2012-2025 Vincent Damewood
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

#include <SilikoCore/InfixParser.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/Lexer.h>

static SilikoSyntaxTreeNode *GetExpression(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExpressionRest(SilikoLexer *lexer, SilikoSyntaxTreeNode*);
static SilikoSyntaxTreeNode *GetTerm(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetTermRest(SilikoLexer *lexer, SilikoSyntaxTreeNode*);
static SilikoSyntaxTreeNode *GetExponent(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetExponentRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetRoll(SilikoLexer *lexe);
static SilikoSyntaxTreeNode *GetRollRest(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetUnsignedNumber(SilikoLexer *lexer);
static SilikoSyntaxTreeNode *GetFunctionCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal);

static SilikoSyntaxTreeNode *GetExpression(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftOperand = GetTerm(lexer);
	if (!leftOperand)
		return NULL;

	return GetExpressionRest(lexer, leftOperand);
}

#include <stdio.h>

static SilikoSyntaxTreeNode *GetExpressionRest(SilikoLexer *lexer, SilikoSyntaxTreeNode *leftSide)
{
	char *operation = NULL;
	switch (SilikoLexerGetCurrent(lexer).Type)
	{
	case '+':
		operation = "add";
		break;
	case '-':
		operation = "subtract";
		break;
	default:
		return leftSide;
	}
	SilikoLexerAdvance(lexer);

	SilikoSyntaxTreeNode *branchNode = SilikoSyntaxTreeNewBranch(operation);
	if (!branchNode)
	{
		SilikoSyntaxTreeDelete(leftSide);
		return NULL;
	}
	SilikoSyntaxTreePushRight(branchNode, leftSide);

	SilikoSyntaxTreeNode *nextOperand = GetTerm(lexer);
	if (!nextOperand)
	{
		SilikoSyntaxTreeDelete(branchNode);
		return NULL;
	}
	SilikoSyntaxTreePushRight(branchNode, nextOperand);

	return GetExpressionRest(lexer, branchNode);
}

static SilikoSyntaxTreeNode *GetTerm(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftOperand = GetExponent(lexer);
	if (!leftOperand)
		return NULL;

	return GetTermRest(lexer, leftOperand);
}

static SilikoSyntaxTreeNode *GetTermRest(SilikoLexer *lexer, SilikoSyntaxTreeNode *leftSide)
{
	char *operation = NULL;
	switch (SilikoLexerGetCurrent(lexer).Type)
	{
	case '*':
		operation = "multiply";
		break;
	case '/':
		operation = "divide";
		break;
	default:
		return leftSide;
	}
	SilikoLexerAdvance(lexer);

	SilikoSyntaxTreeNode *branchNode = SilikoSyntaxTreeNewBranch(operation);
	if (!branchNode)
	{
		SilikoSyntaxTreeDelete(leftSide);
		return NULL;
	}
	SilikoSyntaxTreePushRight(branchNode, leftSide);

	SilikoSyntaxTreeNode *nextOperand = GetExponent(lexer);
	if (!nextOperand)
	{
		SilikoSyntaxTreeDelete(branchNode);
		return NULL;
	}
	SilikoSyntaxTreePushRight(branchNode, nextOperand);

	return GetTermRest(lexer, branchNode);
}

static SilikoSyntaxTreeNode *GetExponent(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode * rVal = NULL;

	if (!(leftValue = GetRoll(lexer)))
		return NULL;

	if (!(rest = GetExponentRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}

	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("power")))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
}

static SilikoSyntaxTreeNode *GetExponentRest(SilikoLexer *lexer)
{
	if (SilikoLexerGetCurrent(lexer).Type == '^')
	{
		SilikoLexerAdvance(lexer);

		switch (SilikoLexerGetCurrent(lexer).Type)
		{
		case SILIKO_TOK_INTEGER:
		case SILIKO_TOK_FLOAT:
		case '-':
		case SILIKO_TOK_ID:
		case '(':
			return GetExponent(lexer);
		default:
			return SilikoSyntaxTreeNewError();
		}
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetRoll(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *leftValue = NULL;
	SilikoSyntaxTreeNode *rest = NULL;
	SilikoSyntaxTreeNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		return NULL;

	if (!(rest = GetRollRest(lexer)))
	{
		SilikoSyntaxTreeDelete(leftValue);
		return NULL;
	}


	if (SilikoSyntaxTreeGetType(rest) == SILIKO_AST_NOTHING)
	{
		SilikoSyntaxTreeDelete(rest);
		return leftValue;
	}

	if (!(rVal = SilikoSyntaxTreeNewBranch("dice")))
	{
		SilikoSyntaxTreeDelete(leftValue);
		SilikoSyntaxTreeDelete(rest);
		return NULL;
	}

	SilikoSyntaxTreePushRight(rVal, leftValue);
	SilikoSyntaxTreePushRight(rVal, rest);
	return rVal;
}

static SilikoSyntaxTreeNode *GetRollRest(SilikoLexer *lexer)
{
	long long int value;

	if(SilikoLexerGetCurrent(lexer).Type == 'd')
	{
		SilikoLexerAdvance(lexer);
		if (SilikoLexerGetCurrent(lexer).Type != SILIKO_TOK_INTEGER)
			return SilikoSyntaxTreeNewError();

		value = SilikoLexerGetCurrent(lexer).Integer;
		SilikoLexerAdvance(lexer);
		return SilikoSyntaxTreeNewFromInteger(value);
	}
	else
	{
		return SilikoSyntaxTreeNewNothing();
	}
}

static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *value;

	switch(SilikoLexerGetCurrent(lexer).Type)
	{
	case '-':
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetNumber(lexer);
	case '(':
		SilikoLexerAdvance(lexer);
		if (!(value = GetExpression(lexer)))
			return NULL;

		if (SilikoLexerGetCurrent(lexer).Type != ')')
		{
			SilikoSyntaxTreeDelete(value);
			return SilikoSyntaxTreeNewError();
		}
		SilikoLexerAdvance(lexer);
		return value;
	case SILIKO_TOK_ID:
		return GetFunctionCall(lexer);
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode * UnsignedNumber = NULL;

	switch (SilikoLexerGetCurrent(lexer).Type)
	{
	case SILIKO_TOK_INTEGER:
	case SILIKO_TOK_FLOAT:
		return GetUnsignedNumber(lexer);
	case '-':
		SilikoLexerAdvance(lexer);
		if (!(UnsignedNumber = GetUnsignedNumber(lexer)))
			return NULL;

		if (!SilikoSyntaxTreeNegate(UnsignedNumber))
		{
			SilikoSyntaxTreeDelete(UnsignedNumber);
			return SilikoSyntaxTreeNewError();
		}

		return UnsignedNumber;
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetUnsignedNumber(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	switch (SilikoLexerGetCurrent(lexer).Type)
	{
	case SILIKO_TOK_INTEGER:
		rVal = SilikoSyntaxTreeNewFromInteger(SilikoLexerGetCurrent(lexer).Integer);
		SilikoLexerAdvance(lexer);
		break;
	case SILIKO_TOK_FLOAT:
		rVal = SilikoSyntaxTreeNewFromFloat(SilikoLexerGetCurrent(lexer).Float);
		SilikoLexerAdvance(lexer);
		break;
	default:
		rVal = SilikoSyntaxTreeNewError();
	}

	return rVal;
}

static SilikoSyntaxTreeNode *GetFunctionCall(SilikoLexer *lexer)
{
	SilikoSyntaxTreeNode *rVal = NULL;

	if (SilikoLexerGetCurrent(lexer).Type != SILIKO_TOK_ID)
		return SilikoSyntaxTreeNewError();

	rVal = SilikoSyntaxTreeNewBranch(SilikoLexerGetCurrent(lexer).Id);
	SilikoLexerAdvance(lexer);

	if (SilikoLexerGetCurrent(lexer).Type != '(')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerAdvance(lexer);

	GetArguments(lexer, rVal);

	if (SilikoLexerGetCurrent(lexer).Type != ')')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerAdvance(lexer);

	return rVal;
}

static void GetArguments(SilikoLexer *lexer, SilikoSyntaxTreeNode *rVal)
{
	while(-1)
	{
		SilikoSyntaxTreeNode *Expression = GetExpression(lexer);
		SilikoSyntaxTreePushRight(rVal, Expression);

		if (SilikoSyntaxTreeIsError(Expression) || SilikoLexerGetCurrent(lexer).Type == ')')
		{
			break;
		}
		else if (SilikoLexerGetCurrent(lexer).Type != ',')
		{
			SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
			break;
		}
		SilikoLexerAdvance(lexer);
	}
}


SilikoSyntaxTreeNode *SilikoParseInfix(SilikoDataSource *Input)
{
	SilikoSyntaxTreeNode *rVal = NULL;
	SilikoLexer *lexer = NULL;

	if (!(lexer = SilikoLexerNew(Input)))
		return NULL;

	rVal = GetExpression(lexer);

	if (SilikoLexerGetCurrent(lexer).Type != SILIKO_TOK_EOL)
	{
		SilikoSyntaxTreeDelete(rVal);
		rVal = SilikoSyntaxTreeNewError();
	}
	SilikoLexerDelete(lexer);
	return rVal;
}
