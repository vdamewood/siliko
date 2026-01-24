// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Siliko.

// Siliko is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Siliko is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Siliko. If not, see
// <http://www.gnu.org/licenses/>.

#include <stdlib.h>

#include <SilikoCore/Input.h>
#include <SilikoCore/Lexer.h>
#include <SilikoCore/Node.h>
#include <SilikoCore/ParseInfix.h>
#include <SilikoCore/Token.h>
#include <SilikoCore/Value.h>

static SilikoNode *GetExpression(SilikoLexer *lexer);
static SilikoNode *GetExpressionRest(SilikoLexer *lexer, SilikoNode*);
static SilikoNode *GetTerm(SilikoLexer *lexer);
static SilikoNode *GetTermRest(SilikoLexer *lexer, SilikoNode*);
static SilikoNode *GetExponent(SilikoLexer *lexer);
static SilikoNode *GetExponentRest(SilikoLexer *lexer);
static SilikoNode *GetRoll(SilikoLexer *lexe);
static SilikoNode *GetRollRest(SilikoLexer *lexer);
static SilikoNode *GetAtom(SilikoLexer *lexer);
static SilikoNode *GetNumber(SilikoLexer *lexer);
static SilikoNode *GetUnsignedNumber(SilikoLexer *lexer);
static SilikoNode *GetFunctionCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoNode *rVal);

static SilikoNode *GetExpression(SilikoLexer *lexer)
{
	SilikoNode *leftOperand = GetTerm(lexer);
	if (!leftOperand)
		return NULL;

	return GetExpressionRest(lexer, leftOperand);
}

static SilikoNode *GetExpressionRest(SilikoLexer *lexer, SilikoNode *leftSide)
{
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)) != SilikoTokenCharacter)
		return leftSide;

	char *operation = NULL;
	switch (SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
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

	SilikoNode *branchNode = SilikoNodeCreateBranch(operation);
	if (!branchNode)
	{
		SilikoNodeDestroy(leftSide);
		return NULL;
	}
	SilikoNodePushRight(branchNode, leftSide);

	SilikoNode *nextOperand = GetTerm(lexer);
	if (!nextOperand)
	{
		SilikoNodeDestroy(branchNode);
		return NULL;
	}
	SilikoNodePushRight(branchNode, nextOperand);

	return GetExpressionRest(lexer, branchNode);
}

static SilikoNode *GetTerm(SilikoLexer *lexer)
{
	SilikoNode *leftOperand = GetExponent(lexer);
	if (!leftOperand)
		return NULL;

	return GetTermRest(lexer, leftOperand);
}

static SilikoNode *GetTermRest(SilikoLexer *lexer, SilikoNode *leftSide)
{
	char *operation = NULL;
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)) != SilikoTokenCharacter)
		return leftSide;

	switch (SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
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

	SilikoNode *branchNode = SilikoNodeCreateBranch(operation);
	if (!branchNode)
	{
		SilikoNodeDestroy(leftSide);
		return NULL;
	}
	SilikoNodePushRight(branchNode, leftSide);

	SilikoNode *nextOperand = GetExponent(lexer);
	if (!nextOperand)
	{
		SilikoNodeDestroy(branchNode);
		return NULL;
	}
	SilikoNodePushRight(branchNode, nextOperand);

	return GetTermRest(lexer, branchNode);
}

static SilikoNode *GetExponent(SilikoLexer *lexer)
{
	SilikoNode *leftValue = NULL;
	SilikoNode *rest = NULL;
	SilikoNode * rVal = NULL;

	if (!(leftValue = GetRoll(lexer)))
		return NULL;

	if (!(rest = GetExponentRest(lexer)))
	{
		SilikoNodeDestroy(leftValue);
		return NULL;
	}

	if (SilikoNodeGetStatus(rest) == SilikoNodeNothing)
	{
		SilikoNodeDestroy(rest);
		return leftValue;
	}

	if (!(rVal = SilikoNodeCreateBranch("power")))
	{
		SilikoNodeDestroy(leftValue);
		SilikoNodeDestroy(rest);
		return NULL;
	}

	SilikoNodePushRight(rVal, leftValue);
	SilikoNodePushRight(rVal, rest);
	return rVal;
}

static SilikoNode *GetExponentRest(SilikoLexer *lexer)
{
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
				!= SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
				!= '^')
		return SilikoNodeCreateNothing();

	SilikoLexerAdvance(lexer);

	switch (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
	case SilikoTokenId:
		return GetExponent(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
		{
		case '-':
		case '(':
			return GetExponent(lexer);
		default:
			;
		}
	default:
		;
	}
	return SilikoNodeCreateFromError(SilikoErrorSyntax);
}

static SilikoNode *GetRoll(SilikoLexer *lexer)
{
	SilikoNode *leftValue = NULL;
	SilikoNode *rest = NULL;
	SilikoNode *rVal;

	if (!(leftValue = GetAtom(lexer)))
		return NULL;

	if (!(rest = GetRollRest(lexer)))
	{
		SilikoNodeDestroy(leftValue);
		return NULL;
	}


	if (SilikoNodeGetStatus(rest) == SilikoNodeNothing)
	{
		SilikoNodeDestroy(rest);
		return leftValue;
	}

	if (!(rVal = SilikoNodeCreateBranch("dice")))
	{
		SilikoNodeDestroy(leftValue);
		SilikoNodeDestroy(rest);
		return NULL;
	}

	SilikoNodePushRight(rVal, leftValue);
	SilikoNodePushRight(rVal, rest);
	return rVal;
}

static SilikoNode *GetRollRest(SilikoLexer *lexer)
{
	if(SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
				!= SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
				!= 'd')
		return SilikoNodeCreateNothing();

	SilikoLexerAdvance(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenInteger)
		return SilikoNodeCreateFromError(SilikoErrorSyntax);

	long long int value = SilikoTokenGetInteger(
		SilikoLexerGetToken(lexer));
	SilikoLexerAdvance(lexer);
	return SilikoNodeCreateFromInteger(value);
}

static SilikoNode *GetAtom(SilikoLexer *lexer)
{
	switch(SilikoTokenGetStatus(SilikoLexerGetToken(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
		return GetNumber(lexer);
	case SilikoTokenId:
		return GetFunctionCall(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
		{
		case '-':
			return GetNumber(lexer);
		case '(':
		{
			SilikoLexerAdvance(lexer);

			SilikoNode *expression = GetExpression(lexer);
			if (!expression)
				return NULL;

			if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
					!= SilikoTokenCharacter
				|| SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
					!= ')')
			{
				SilikoNodeDestroy(expression);
				return SilikoNodeCreateFromError(SilikoErrorSyntax);
			}
			SilikoLexerAdvance(lexer);
			return expression;
		}
		default:
			;
		}
	default:
		;
	}
	return SilikoNodeCreateFromError(SilikoErrorSyntax);
}

static SilikoNode *GetNumber(SilikoLexer *lexer)
{
	switch (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
		return GetUnsignedNumber(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
		{
		case '-':
		{
			SilikoLexerAdvance(lexer);

			SilikoNode *number = GetUnsignedNumber(lexer);
			if (!number)
				return NULL;

			if (!SilikoNodeNegate(number))
			{
				SilikoNodeDestroy(number);
				return SilikoNodeCreateFromError(SilikoErrorSyntax);
			}

			return number;
		}
		default:
			;
		}
	default:
		;
	}
		return SilikoNodeCreateFromError(SilikoErrorSyntax);
}

static SilikoNode *GetUnsignedNumber(SilikoLexer *lexer)
{
	switch (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)))
	{
	case SilikoTokenInteger:
	{
		SilikoNode *integer =
			SilikoNodeCreateFromInteger(
				SilikoTokenGetInteger(
					SilikoLexerGetToken(lexer)));
		if (integer)
			SilikoLexerAdvance(lexer);
		return integer;
	}
	case SilikoTokenReal:
	{
		SilikoNode *real =
			SilikoNodeCreateFromReal(
				SilikoTokenGetReal(
					SilikoLexerGetToken(lexer)));
		if (real)
			SilikoLexerAdvance(lexer);
		return real;
	}
	default:
		return SilikoNodeCreateFromError(SilikoErrorSyntax);
	}
}

static SilikoNode *GetFunctionCall(SilikoLexer *lexer)
{
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenId)
		return SilikoNodeCreateFromError(SilikoErrorSyntax);

	SilikoNode *rVal =
		SilikoNodeCreateBranch(
			SilikoTokenGetId(
				SilikoLexerGetToken(lexer)));
	SilikoLexerAdvance(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
			!= '(')
	{
		SilikoNodePushRight(rVal, SilikoNodeCreateFromError(SilikoErrorSyntax));
		return rVal;
	}
	SilikoLexerAdvance(lexer);

	GetArguments(lexer, rVal);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
			!= ')')
	{
		SilikoNodePushRight(rVal, SilikoNodeCreateFromError(SilikoErrorSyntax));
		return rVal;
	}
	SilikoLexerAdvance(lexer);

	return rVal;
}

static void GetArguments(SilikoLexer *lexer, SilikoNode *rVal)
{
	while(-1)
	{
		SilikoNode *Expression = GetExpression(lexer);
		SilikoNodePushRight(rVal, Expression);

		if ((SilikoNodeGetStatus(Expression) == SilikoNodeLeaf
				&& SilikoValueGetStatus(SilikoNodeGetValue(Expression))
					== SilikoValueError)
			|| (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
				== SilikoTokenCharacter
				&& SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
					== ')'))
		{
			break;
		}
		else if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
				!= ',')
		{
			SilikoNodePushRight(rVal, SilikoNodeCreateFromError(SilikoErrorSyntax));
			break;
		}
		SilikoLexerAdvance(lexer);
	}
}


SilikoNode *SilikoParseInfix(SilikoInput *Input)
{
	if (!Input)
		return NULL;

	SilikoLexer *lexer = SilikoLexerCreate(Input);

	if (!lexer)
		return NULL;

	SilikoNode *rVal = GetExpression(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer)) != SilikoTokenEndOfInput)
	{
		SilikoNodeDestroy(rVal);
		rVal = SilikoNodeCreateFromError(SilikoErrorSyntax);
	}
	SilikoLexerDestroy(lexer);
	return rVal;
}
