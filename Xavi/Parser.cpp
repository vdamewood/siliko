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

#include <typeinfo>

#include "Parser.hpp"
#include "SyntaxTree.hpp"

static Xavi::SyntaxTreeNode *GetExpr0(Xavi::Lexer *lexer);
static Xavi::BranchNode     *GetExpr0r(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetExpr1(Xavi::Lexer *lexer);
static Xavi::BranchNode     *GetExpr1r(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetExpr2(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetExpr2lf(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetExpr3(Xavi::Lexer *lexe);
static Xavi::SyntaxTreeNode *GetExpr3lf(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetAtom(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetNumber(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetUNumber(Xavi::Lexer *lexer);
static Xavi::SyntaxTreeNode *GetFCall(Xavi::Lexer *lexer);
static Xavi::BranchNode *GetArguments(Xavi::Lexer *lexer);

static Xavi::SyntaxTreeNode *GetExpr0(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *Left = GetExpr1(lexer);
	Xavi::BranchNode *Rest = GetExpr0r(lexer);

	if (!Rest)
		return Left;

	if (Rest->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

static Xavi::BranchNode *GetExpr0r(Xavi::Lexer *lexer)
{
	std::string FunctionId;
	switch (lexer->GetToken().GetType())
	{
	case '+':
		FunctionId = "add";
		break;
	case '-':
		FunctionId = "subtract";
		break;
	default:
		return 0;
	}

	lexer->Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr1(lexer));
	Xavi::BranchNode *Rest = GetExpr0r(lexer);

	if (Rest)
	{
		Rest->GraftLeft(Branch);
		return Rest;
	}
	else
	{
		return Branch;
	}
}

static Xavi::SyntaxTreeNode *GetExpr1(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *Left = GetExpr2(lexer);
	Xavi::BranchNode *Rest = GetExpr1r(lexer);

	if (!Rest)
		return Left;

	if (Rest->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

static Xavi::BranchNode *GetExpr1r(Xavi::Lexer *lexer)
{
	std::string FunctionId;
	switch (lexer->GetToken().GetType())
	{
		case '*':
			FunctionId = "multiply";
			break;
		case '/':
			FunctionId = "divide";
			break;
		default:
			return 0;
	}

	lexer->Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr2(lexer));
	Xavi::BranchNode *Rest = GetExpr1r(lexer);

	if (Rest)
	{
		Rest->GraftLeft(Branch);
		return Rest;
	}
	else
	{
		return Branch;
	}
}

static Xavi::SyntaxTreeNode *GetExpr2(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *leftValue = GetExpr3(lexer);
	Xavi::SyntaxTreeNode *rest = GetExpr2lf(lexer);

	if (!rest)
		return leftValue;

	Xavi::BranchNode *rVal = new Xavi::BranchNode("power");
	rVal->PushRight(leftValue);
	rVal->PushRight(rest);
	return rVal;
}

static Xavi::SyntaxTreeNode *GetExpr2lf(Xavi::Lexer *lexer)
{
	if (lexer->GetToken().GetType() != '^')
		return 0;

	lexer->Next();

	switch (lexer->GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
	case '-':
	case Xavi::Token::ID:
	case '(':
		return GetExpr2(lexer);
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetExpr3(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *leftValue = GetAtom(lexer);
	Xavi::SyntaxTreeNode *rest = GetExpr3lf(lexer);

	if (!rest)
		return leftValue;

	Xavi::BranchNode *rVal = new Xavi::BranchNode("dice");
	rVal->PushRight(leftValue);
	rVal->PushRight(rest);
	return rVal;
}

static Xavi::SyntaxTreeNode *GetExpr3lf(Xavi::Lexer *lexer)
{
	if(lexer->GetToken().GetType() != 'd')
		return 0;

	lexer->Next();

	if (lexer->GetToken().GetType() == Xavi::Token::INTEGER)
	{
		Xavi::SyntaxTreeNode *rVal = new Xavi::IntegerNode(lexer->GetToken().GetIntegerValue());
		lexer->Next();
		return rVal;
	}
	else
	{
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetAtom(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *value;

	switch(lexer->GetToken().GetType())
	{
	case '-':
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetNumber(lexer);
	case '(':
		lexer->Next();
		value = GetExpr0(lexer);

		if (lexer->GetToken().GetType() != ')')
		{
			delete value;
			return new Xavi::SyntaxErrorNode();
		}

		lexer->Next();
		return value;
	case Xavi::Token::ID:
		return GetFCall(lexer);
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetNumber(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (lexer->GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetUNumber(lexer);
	case '-':
		lexer->Next();
		rVal = GetUNumber(lexer);
		rVal->Negate();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetUNumber(Xavi::Lexer *lexer)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (lexer->GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
		rVal = new Xavi::IntegerNode(lexer->GetToken().GetIntegerValue());
		lexer->Next();
		return rVal;
	case Xavi::Token::FLOAT:
		rVal = new Xavi::FloatNode(lexer->GetToken().GetFloatValue());
		lexer->Next();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetFCall(Xavi::Lexer *lexer)
{
	if (lexer->GetToken().GetType() != Xavi::Token::ID)
		return new Xavi::SyntaxErrorNode();

	std::string id = lexer->GetToken().GetIdValue();
	lexer->Next();

	if (lexer->GetToken().GetType() != '(')
		return new Xavi::SyntaxErrorNode();

	lexer->Next();

	Xavi::SyntaxTreeNode *rVal = GetArguments(lexer);
	if (!rVal)
		return new Xavi::SyntaxErrorNode();

	if (typeid(*rVal) == typeid(Xavi::SyntaxErrorNode))
		return rVal;

	if (lexer->GetToken().GetType() != ')' || typeid(*rVal) != typeid(Xavi::BranchNode))
	{
		delete rVal;
		return new Xavi::SyntaxErrorNode();
	}

	static_cast<Xavi::BranchNode *>(rVal)->SetId(id);
	lexer->Next();

	return rVal;
}

static Xavi::BranchNode *GetArguments(Xavi::Lexer *lexer)
{
	Xavi::BranchNode *rVal = new Xavi::BranchNode();

	while(true)
	{
		Xavi::SyntaxTreeNode *Expression = GetExpr0(lexer);
		rVal->PushRight(Expression);

		if (typeid(*Expression) == typeid(Xavi::SyntaxErrorNode)
			|| lexer->GetToken().GetType() == ')')
		{
			break;
		}
		else if (lexer->GetToken().GetType() != ',')
		{
			rVal->PushRight(new Xavi::SyntaxErrorNode());
			break;
		}
		lexer->Next();
	}

	return rVal;
}

XaviValue Xavi::Parse(Xavi::Lexer *lexer)
{
	XaviValue rVal;
	if (lexer->GetToken().GetType() == Token::EOL)
	{
		rVal.status = XS_INTEGER;
		rVal.i = 0;
		return rVal;
	}

	Xavi::SyntaxTreeNode *syntaxTree = GetExpr0(lexer);
	if (typeid(syntaxTree) == typeid(Xavi::SyntaxErrorNode)
		|| lexer->GetToken().GetType() != Token::EOL)
	{
		rVal.status = XE_SYNTAX;
		rVal.i = 0;
	}
	else
	{
		rVal = syntaxTree->GetValue();
	}
	delete syntaxTree;
	return rVal;
}
