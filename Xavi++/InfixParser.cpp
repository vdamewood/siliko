/*
 * InfixParser.cpp: Recursive-descent infix parser
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

#include <typeinfo>

#include "Lexer.hpp"
#include "InfixParser.hpp"

static Xavi::SyntaxTreeNode *GetExpr0(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr0r(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr1(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr1r(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr2(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr2lf(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr3(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetExpr3lf(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetAtom(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetNumber(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetUNumber(Xavi::Lexer&);
static Xavi::SyntaxTreeNode *GetFCall(Xavi::Lexer&);
static void                  GetArguments(Xavi::Lexer&, Xavi::BranchNode&);


Xavi::SyntaxTreeNode* Xavi::ParseInfix(Xavi::DataSource* NewSource)
{
	Xavi::Lexer MyLexer(NewSource);
	Xavi::SyntaxTreeNode* rVal = 0;

	if (MyLexer.GetToken().Type() == Token::EOL)
	{
		rVal = new Xavi::IntegerNode(0);
	}
	else
	{
		rVal = GetExpr0(MyLexer);
		if (MyLexer.GetToken().Type() != Token::EOL
			&& typeid(rVal) != typeid(Xavi::SyntaxErrorNode))
		{
			delete rVal;
			rVal = new SyntaxErrorNode();
		}
	}

	return rVal;
}

static Xavi::SyntaxTreeNode *GetExpr0(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *Left = GetExpr1(MyLexer);
	Xavi::SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return Left;
	}

	if (reinterpret_cast<Xavi::BranchNode*>(Rest)->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

static Xavi::SyntaxTreeNode *GetExpr0r(Xavi::Lexer& MyLexer)
{
	const char *FunctionId;
	switch (MyLexer.GetToken().Type())
	{
	case '+':
		FunctionId = "add";
		break;
	case '-':
		FunctionId = "subtract";
		break;
	default:
		return new Xavi::NothingNode();
	}

	MyLexer.Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr1(MyLexer));
	Xavi::SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return Branch;
	}
	else
	{
		reinterpret_cast<Xavi::BranchNode*>(Rest)->GraftLeft(Branch);
		return Rest;
	}
}

static Xavi::SyntaxTreeNode *GetExpr1(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *Left = GetExpr2(MyLexer);
	Xavi::SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return Left;
	}

	if (reinterpret_cast<Xavi::BranchNode*>(Rest)->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

static Xavi::SyntaxTreeNode *GetExpr1r(Xavi::Lexer& MyLexer)
{
	const char *FunctionId;
	switch (MyLexer.GetToken().Type())
	{
		case '*':
			FunctionId = "multiply";
			break;
		case '/':
			FunctionId = "divide";
			break;
		default:
			return new Xavi::NothingNode();
	}

	MyLexer.Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr2(MyLexer));
	Xavi::SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return Branch;
	}
	else
	{
		reinterpret_cast<Xavi::BranchNode*>(Rest)->GraftLeft(Branch);
		return Rest;
	}
}


static Xavi::SyntaxTreeNode *GetExpr2(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *leftValue = GetExpr3(MyLexer);
	Xavi::SyntaxTreeNode *Rest = GetExpr2lf(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return leftValue;
	}

	Xavi::BranchNode *rVal = new Xavi::BranchNode("power");
	rVal->PushRight(leftValue);
	rVal->PushRight(Rest);
	return rVal;
}

static Xavi::SyntaxTreeNode *GetExpr2lf(Xavi::Lexer& MyLexer)
{
	if (MyLexer.GetToken().Type() != '^')
		return new Xavi::NothingNode();

	MyLexer.Next();

	switch (MyLexer.GetToken().Type())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
	case '-':
	case Xavi::Token::ID:
	case '(':
		return GetExpr2(MyLexer);
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetExpr3(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *leftValue = GetAtom(MyLexer);
	Xavi::SyntaxTreeNode *Rest = GetExpr3lf(MyLexer);

	if (typeid(*Rest) == typeid(Xavi::NothingNode))
	{
		delete Rest;
		return leftValue;
	}

	Xavi::BranchNode *rVal = new Xavi::BranchNode("dice");
	rVal->PushRight(leftValue);
	rVal->PushRight(Rest);
	return rVal;
}

static Xavi::SyntaxTreeNode *GetExpr3lf(Xavi::Lexer& MyLexer)
{
	if(MyLexer.GetToken().Type() != 'd')
		return new Xavi::NothingNode();

	MyLexer.Next();

	if (MyLexer.GetToken().Type() == Xavi::Token::INTEGER)
	{
		Xavi::SyntaxTreeNode *rVal = new Xavi::IntegerNode(MyLexer.GetToken().Integer());
		MyLexer.Next();
		return rVal;
	}
	else
	{
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetAtom(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *value;

	switch(MyLexer.GetToken().Type())
	{
	case '-':
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetNumber(MyLexer);
	case '(':
		MyLexer.Next();
		value = GetExpr0(MyLexer);

		if (MyLexer.GetToken().Type() != ')')
		{
			delete value;
			return new Xavi::SyntaxErrorNode();
		}

		MyLexer.Next();
		return value;
	case Xavi::Token::ID:
		return GetFCall(MyLexer);
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetNumber(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().Type())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetUNumber(MyLexer);
	case '-':
		MyLexer.Next();
		rVal = GetUNumber(MyLexer);
		rVal->Negate();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetUNumber(Xavi::Lexer& MyLexer)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().Type())
	{
	case Xavi::Token::INTEGER:
		rVal = new Xavi::IntegerNode(MyLexer.GetToken().Integer());
		MyLexer.Next();
		return rVal;
	case Xavi::Token::FLOAT:
		rVal = new Xavi::FloatNode(MyLexer.GetToken().Float());
		MyLexer.Next();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

static Xavi::SyntaxTreeNode *GetFCall(Xavi::Lexer& MyLexer)
{
	if (MyLexer.GetToken().Type() != Xavi::Token::ID)
		return new Xavi::SyntaxErrorNode();

	Xavi::BranchNode *rVal = new Xavi::BranchNode(MyLexer.GetToken().Id());
	MyLexer.Next();

	if (MyLexer.GetToken().Type() != '(')
	{
		rVal->PushRight(new Xavi::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	GetArguments(MyLexer, *rVal);

	if (MyLexer.GetToken().Type() != ')')
	{
		rVal->PushRight(new Xavi::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	return rVal;
}

static void GetArguments(Xavi::Lexer& MyLexer, Xavi::BranchNode& rVal)
{
	while(true)
	{
		Xavi::SyntaxTreeNode *Expression = GetExpr0(MyLexer);
		rVal.PushRight(Expression);

		if (typeid(*Expression) == typeid(Xavi::SyntaxErrorNode)
			|| MyLexer.GetToken().Type() == ')')
		{
			break;
		}
		else if (MyLexer.GetToken().Type() != ',')
		{
			rVal.PushRight(new Xavi::SyntaxErrorNode());
			break;
		}
		MyLexer.Next();
	}
}
