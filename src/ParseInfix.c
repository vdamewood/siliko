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
static SilikoNode *GetExpressionRest(
	SilikoLexer *lexer,
	SilikoNode *branch);
static SilikoNode *GetTerm(SilikoLexer *lexer);
static SilikoNode *GetTermRest(
	SilikoLexer *lexer,
	SilikoNode *branch);
static SilikoNode *GetExponent(SilikoLexer *lexer);
static SilikoNode *GetExponentRest(SilikoLexer *lexer);
static SilikoNode *GetRoll(SilikoLexer *lexe);
static SilikoNode *GetRollRest(SilikoLexer *lexer);
static SilikoNode *GetAtom(SilikoLexer *lexer);
static SilikoNode *GetNumber(SilikoLexer *lexer);
static SilikoNode *GetUnsignedNumber(SilikoLexer *lexer);
static SilikoNode *GetFunctionCall(SilikoLexer *lexer);
static void GetArguments(SilikoLexer *lexer, SilikoNode *branch);

SilikoNode *SilikoParseInfix(SilikoInput *input)
{
	if (!input)
		return NULL;

	SilikoLexer *lexer = SilikoLexerCreate(input, -1, -1);

	if (!lexer)
		return NULL;

	SilikoNode *result = GetExpression(lexer);

	if (SilikoTokenGetStatus(
		SilikoLexerGetToken(lexer)) != SilikoTokenEndOfInput)
	{
		SilikoNodeDestroy(result);
		result = SilikoNodeCreateFromError(SilikoErrorSyntax);
	}
	SilikoLexerDestroy(lexer);
	return result;
}

static SilikoNode *GetExpression(SilikoLexer *lexer)
{
	SilikoNode *left_operand = GetTerm(lexer);
	if (!left_operand)
		return NULL;

	return GetExpressionRest(lexer, left_operand);
}

static SilikoNode *GetExpressionRest(
	SilikoLexer *lexer,
	SilikoNode *left_side)
{
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter)
		return left_side;

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
		return left_side;
	}
	SilikoLexerAdvance(lexer);

	SilikoNode *branch_node = SilikoNodeCreateBranch(operation);
	if (!branch_node)
	{
		SilikoNodeDestroy(left_side);
		return NULL;
	}
	SilikoNodePushRight(branch_node, left_side);

	SilikoNode *next_operand = GetTerm(lexer);
	if (!next_operand)
	{
		SilikoNodeDestroy(branch_node);
		return NULL;
	}
	SilikoNodePushRight(branch_node, next_operand);

	return GetExpressionRest(lexer, branch_node);
}

static SilikoNode *GetTerm(SilikoLexer *lexer)
{
	SilikoNode *leftOperand = GetExponent(lexer);
	if (!leftOperand)
		return NULL;

	return GetTermRest(lexer, leftOperand);
}

static SilikoNode *GetTermRest(
	SilikoLexer *lexer,
	SilikoNode *left_side)
{
	char *operation = NULL;
	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter)
		return left_side;

	switch (SilikoTokenGetCharacter(SilikoLexerGetToken(lexer)))
	{
	case '*':
		operation = "multiply";
		break;
	case '/':
		operation = "divide";
		break;
	default:
		return left_side;
	}
	SilikoLexerAdvance(lexer);

	SilikoNode *branch_node = SilikoNodeCreateBranch(operation);
	if (!branch_node)
	{
		SilikoNodeDestroy(left_side);
		return NULL;
	}
	SilikoNodePushRight(branch_node, left_side);

	SilikoNode *next_operand = GetExponent(lexer);
	if (!next_operand)
	{
		SilikoNodeDestroy(branch_node);
		return NULL;
	}
	SilikoNodePushRight(branch_node, next_operand);

	return GetTermRest(lexer, branch_node);
}

static SilikoNode *GetExponent(SilikoLexer *lexer)
{
	SilikoNode *left_value = GetRoll(lexer);
	if (!left_value)
		return NULL;

	SilikoNode *rest = GetExponentRest(lexer);
	if (!rest)
	{
		SilikoNodeDestroy(left_value);
		return NULL;
	}

	if (SilikoNodeGetStatus(rest) == SilikoNodeNothing)
	{
		SilikoNodeDestroy(rest);
		return left_value;
	}

	SilikoNode *branch = SilikoNodeCreateBranch("power");
	if (!branch)
	{
		SilikoNodeDestroy(left_value);
		SilikoNodeDestroy(rest);
		return NULL;
	}

	SilikoNodePushRight(branch, left_value);
	SilikoNodePushRight(branch, rest);
	return branch;
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
	SilikoNode *left_value = GetAtom(lexer);
	if (!left_value)
		return NULL;

	SilikoNode *rest = GetRollRest(lexer);
	if (!rest)
	{
		SilikoNodeDestroy(left_value);
		return NULL;
	}

	if (SilikoNodeGetStatus(rest) == SilikoNodeNothing)
	{
		SilikoNodeDestroy(rest);
		return left_value;
	}

	SilikoNode *branch = SilikoNodeCreateBranch("dice");
	if (!branch)
	{
		SilikoNodeDestroy(left_value);
		SilikoNodeDestroy(rest);
		return NULL;
	}

	SilikoNodePushRight(branch, left_value);
	SilikoNodePushRight(branch, rest);
	return branch;
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

	long long int value
		= SilikoTokenGetInteger(SilikoLexerGetToken(lexer));
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

			SilikoNodeNegate(number);
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

	SilikoNode *branch =
		SilikoNodeCreateBranch(
			SilikoTokenGetId(
				SilikoLexerGetToken(lexer)));
	SilikoLexerAdvance(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
			!= '(')
	{
		SilikoNodePushRight(
			branch,
			SilikoNodeCreateFromError(SilikoErrorSyntax));
		return branch;
	}
	SilikoLexerAdvance(lexer);

	GetArguments(lexer, branch);

	if (SilikoTokenGetStatus(SilikoLexerGetToken(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetToken(lexer))
			!= ')')
	{
		SilikoNodePushRight(
			branch,
			SilikoNodeCreateFromError(SilikoErrorSyntax));
		return branch;
	}
	SilikoLexerAdvance(lexer);

	return branch;
}

static void GetArguments(SilikoLexer *lexer, SilikoNode *branch)
{
	while(-1)
	{
		SilikoNode *Expression = GetExpression(lexer);
		SilikoNodePushRight(branch, Expression);

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
			SilikoNodePushRight(
				branch,
				SilikoNodeCreateFromError(SilikoErrorSyntax));
			break;
		}
		SilikoLexerAdvance(lexer);
	}
}
