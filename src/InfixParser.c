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

static SilikoSyntaxTreeNode *GetExpressionRest(SilikoLexer *lexer, SilikoSyntaxTreeNode *leftSide)
{
	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)) != SilikoTokenCharacter)
		return leftSide;
	
	char *operation = NULL;
	switch (SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)))
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
	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)) != SilikoTokenCharacter)
		return leftSide;
	
	switch (SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)))
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
	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
				!= SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer))
				!= '^')
		return SilikoSyntaxTreeNewNothing();

	SilikoLexerAdvance(lexer);

	switch (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
	case SilikoTokenId:
		return GetExponent(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)))
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
	return SilikoSyntaxTreeNewError();
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
	if(SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
				!= SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer))
				!= 'd')
		return SilikoSyntaxTreeNewNothing();

	SilikoLexerAdvance(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
			!= SilikoTokenInteger)
		return SilikoSyntaxTreeNewError();

	long long int value = SilikoTokenGetInteger(
		SilikoLexerGetCurrent(lexer));
	SilikoLexerAdvance(lexer);
	return SilikoSyntaxTreeNewFromInteger(value);
}

static SilikoSyntaxTreeNode *GetAtom(SilikoLexer *lexer)
{
	switch(SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
		return GetNumber(lexer);
	case SilikoTokenId:
		return GetFunctionCall(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)))
		{
		case '-':
			return GetNumber(lexer);
		case '(':
		{
			SilikoLexerAdvance(lexer);

			SilikoSyntaxTreeNode *expression = GetExpression(lexer);
			if (!expression)
				return NULL;

			if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
					!= SilikoTokenCharacter
				|| SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer))
					!= ')')
			{
				SilikoSyntaxTreeDelete(expression);
				return SilikoSyntaxTreeNewError();
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
	return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetNumber(SilikoLexer *lexer)
{
	switch (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)))
	{
	case SilikoTokenInteger:
	case SilikoTokenReal:
		return GetUnsignedNumber(lexer);
	case SilikoTokenCharacter:
		switch(SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)))
		{
		case '-':
		{
			SilikoLexerAdvance(lexer);

			SilikoSyntaxTreeNode *number = GetUnsignedNumber(lexer);
			if (!number)
				return NULL;

			if (!SilikoSyntaxTreeNegate(number))
			{
				SilikoSyntaxTreeDelete(number);
				return SilikoSyntaxTreeNewError();
			}

			return number;
		}
		default:
			;
		}
	default:
		;
	}
		return SilikoSyntaxTreeNewError();
}

static SilikoSyntaxTreeNode *GetUnsignedNumber(SilikoLexer *lexer)
{
	switch (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)))
	{
	case SilikoTokenInteger:
	{
		SilikoSyntaxTreeNode *integer =
			SilikoSyntaxTreeNewFromInteger(
				SilikoTokenGetInteger(
					SilikoLexerGetCurrent(lexer)));
		if (integer)
			SilikoLexerAdvance(lexer);
		return integer;
	}
	case SilikoTokenReal:
	{
		SilikoSyntaxTreeNode *real =
			SilikoSyntaxTreeNewFromFloat(
				SilikoTokenGetReal(
					SilikoLexerGetCurrent(lexer)));
		if (real)
			SilikoLexerAdvance(lexer);
		return real;
	}
	default:
		return SilikoSyntaxTreeNewError();
	}
}

static SilikoSyntaxTreeNode *GetFunctionCall(SilikoLexer *lexer)
{
	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
			!= SilikoTokenId)
		return SilikoSyntaxTreeNewError();

	SilikoSyntaxTreeNode *rVal =
		SilikoSyntaxTreeNewBranch(
			SilikoTokenGetId(
				SilikoLexerGetCurrent(lexer)));
	SilikoLexerAdvance(lexer);

	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer))
			!= '(')
	{
		SilikoSyntaxTreePushRight(rVal, SilikoSyntaxTreeNewError());
		return rVal;
	}
	SilikoLexerAdvance(lexer);

	GetArguments(lexer, rVal);

	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer))
			!= SilikoTokenCharacter
		&& SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer))
			!= ')')
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

		if (SilikoSyntaxTreeIsError(Expression)
			|| (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)) == SilikoTokenCharacter
				&& SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)) == ')'))
		{
			break;
		}
		else if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)) != SilikoTokenCharacter
			|| SilikoTokenGetCharacter(SilikoLexerGetCurrent(lexer)) != ',')
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

	if (SilikoTokenGetStatus(SilikoLexerGetCurrent(lexer)) != SilikoTokenEndOfInput)
	{
		SilikoSyntaxTreeDelete(rVal);
		rVal = SilikoSyntaxTreeNewError();
	}
	SilikoLexerDelete(lexer);
	return rVal;
}
