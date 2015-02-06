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

namespace Xavi
{
	class InfixParser
	{
	public:
		InfixParser(Xavi::DataSource *);
		~InfixParser(void);
		void Parse(void);
		SyntaxTreeNode *SyntaxTree(void);
	private:
		Xavi::SyntaxTreeNode        *GetExpr0(void);
		Xavi::BranchNode            *GetExpr0r(void);
		Xavi::SyntaxTreeNode        *GetExpr1(void);
		Xavi::BranchNode            *GetExpr1r(void);
		Xavi::SyntaxTreeNode        *GetExpr2(void);
		Xavi::SyntaxTreeNode        *GetExpr2lf(void);
		Xavi::SyntaxTreeNode        *GetExpr3(void);
		Xavi::SyntaxTreeNode        *GetExpr3lf(void);
		Xavi::SyntaxTreeNode        *GetAtom(void);
		Xavi::SyntaxTreeNode        *GetNumber(void);
		Xavi::SyntaxTreeNode        *GetUNumber(void);
		Xavi::SyntaxTreeNode        *GetFCall(void);
		void                         GetArguments(Xavi::BranchNode &);

		Lexer MyLexer;
		SyntaxTreeNode *MySyntaxTree;
	};
};

Xavi::SyntaxTreeNode *Xavi::ParseInfix(Xavi::DataSource *NewSource)
{
	Xavi::SyntaxTreeNode *rValue;
	Xavi::InfixParser Parser(NewSource);

	Parser.Parse();
	return Parser.SyntaxTree();
}

Xavi::InfixParser::InfixParser(Xavi::DataSource *NewSource)
	: MyLexer(NewSource), MySyntaxTree(0)
{
}

Xavi::InfixParser::~InfixParser(void)
{
	//delete MySyntaxTree;
}

void Xavi::InfixParser::Parse(void)
{
	if (!MySyntaxTree)
	{
		if (MyLexer.GetToken().GetType() == Token::EOL)
		{
			MySyntaxTree = new Xavi::IntegerNode(0);
		}
		else
		{
			MySyntaxTree = GetExpr0();
			if (MyLexer.GetToken().GetType() != Token::EOL
				&& typeid(MySyntaxTree) != typeid(Xavi::SyntaxErrorNode))
			{
				delete MySyntaxTree;
				MySyntaxTree = new SyntaxErrorNode();
			}
		}
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::SyntaxTree(void)
{
	return MySyntaxTree;
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr0(void)
{
	Xavi::SyntaxTreeNode *Left = GetExpr1();
	Xavi::BranchNode *Rest = GetExpr0r();

	if (!Rest)
		return Left;

	if (Rest->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

Xavi::BranchNode *Xavi::InfixParser::GetExpr0r(void)
{
	const char *FunctionId;
	switch (MyLexer.GetToken().GetType())
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

	MyLexer.Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr1());
	Xavi::BranchNode *Rest = GetExpr0r();

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

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr1(void)
{
	Xavi::SyntaxTreeNode *Left = GetExpr2();
	Xavi::BranchNode *Rest = GetExpr1r();

	if (!Rest)
		return Left;

	if (Rest->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Xavi::SyntaxErrorNode();
}

Xavi::BranchNode *Xavi::InfixParser::GetExpr1r(void)
{
	const char *FunctionId;
	switch (MyLexer.GetToken().GetType())
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

	MyLexer.Next();

	Xavi::BranchNode *Branch = new Xavi::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr2());
	Xavi::BranchNode *Rest = GetExpr1r();

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

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr2(void)
{
	Xavi::SyntaxTreeNode *leftValue = GetExpr3();
	Xavi::SyntaxTreeNode *rest = GetExpr2lf();

	if (!rest)
		return leftValue;

	Xavi::BranchNode *rVal = new Xavi::BranchNode("power");
	rVal->PushRight(leftValue);
	rVal->PushRight(rest);
	return rVal;
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr2lf(void)
{
	if (MyLexer.GetToken().GetType() != '^')
		return 0;

	MyLexer.Next();

	switch (MyLexer.GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
	case '-':
	case Xavi::Token::ID:
	case '(':
		return GetExpr2();
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr3(void)
{
	Xavi::SyntaxTreeNode *leftValue = GetAtom();
	Xavi::SyntaxTreeNode *rest = GetExpr3lf();

	if (!rest)
		return leftValue;

	Xavi::BranchNode *rVal = new Xavi::BranchNode("dice");
	rVal->PushRight(leftValue);
	rVal->PushRight(rest);
	return rVal;
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetExpr3lf(void)
{
	if(MyLexer.GetToken().GetType() != 'd')
		return 0;

	MyLexer.Next();

	if (MyLexer.GetToken().GetType() == Xavi::Token::INTEGER)
	{
		Xavi::SyntaxTreeNode *rVal = new Xavi::IntegerNode(MyLexer.GetToken().GetIntegerValue());
		MyLexer.Next();
		return rVal;
	}
	else
	{
		return new Xavi::SyntaxErrorNode();
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetAtom(void)
{
	Xavi::SyntaxTreeNode *value;

	switch(MyLexer.GetToken().GetType())
	{
	case '-':
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetNumber();
	case '(':
		MyLexer.Next();
		value = GetExpr0();

		if (MyLexer.GetToken().GetType() != ')')
		{
			delete value;
			return new Xavi::SyntaxErrorNode();
		}

		MyLexer.Next();
		return value;
	case Xavi::Token::ID:
		return GetFCall();
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetNumber(void)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
	case Xavi::Token::FLOAT:
		return GetUNumber();
	case '-':
		MyLexer.Next();
		rVal = GetUNumber();
		rVal->Negate();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetUNumber(void)
{
	Xavi::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().GetType())
	{
	case Xavi::Token::INTEGER:
		rVal = new Xavi::IntegerNode(MyLexer.GetToken().GetIntegerValue());
		MyLexer.Next();
		return rVal;
	case Xavi::Token::FLOAT:
		rVal = new Xavi::FloatNode(MyLexer.GetToken().GetFloatValue());
		MyLexer.Next();
		return rVal;
	default:
		return new Xavi::SyntaxErrorNode();
	}
}

Xavi::SyntaxTreeNode *Xavi::InfixParser::GetFCall(void)
{
	if (MyLexer.GetToken().GetType() != Xavi::Token::ID)
		return new Xavi::SyntaxErrorNode();

	Xavi::BranchNode *rVal = new Xavi::BranchNode(MyLexer.GetToken().GetIdValue());
	MyLexer.Next();

	if (MyLexer.GetToken().GetType() != '(')
	{
		rVal->PushRight(new Xavi::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	GetArguments(*rVal);

	if (MyLexer.GetToken().GetType() != ')')
	{
		rVal->PushRight(new Xavi::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	return rVal;
}

void Xavi::InfixParser::GetArguments(Xavi::BranchNode &rVal)
{
	while(true)
	{
		Xavi::SyntaxTreeNode *Expression = GetExpr0();
		rVal.PushRight(Expression);

		if (typeid(*Expression) == typeid(Xavi::SyntaxErrorNode)
			|| MyLexer.GetToken().GetType() == ')')
		{
			break;
		}
		else if (MyLexer.GetToken().GetType() != ',')
		{
			rVal.PushRight(new Xavi::SyntaxErrorNode());
			break;
		}
		MyLexer.Next();
	}
}
