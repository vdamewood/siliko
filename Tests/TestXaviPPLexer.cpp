/*
 * TestXavi++Lexer.cpp: Program to test lexer for Xavi++.
 * Copyright 2012, 2015 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstring>

#include <XaviPP/DataSource.hpp>
#include <XaviPP/Lexer.hpp>

void PrintToken(const Xavi::Token &MyToken)
{
	switch (MyToken.Type())
	{
	case Xavi::Token::EOL:
		std::cout << "EOL" << std::endl;
		break;
	case Xavi::Token::INTEGER:
		std::cout << "Integer(" << MyToken.Integer() << ")" << std::endl;
		break;
	case Xavi::Token::FLOAT:
		std::cout << "Float(" << MyToken.Float() << ")" << std::endl;
		break;
	case Xavi::Token::ID:
		std::cout << "ID(" << MyToken.Id() << ")" << std::endl;
		break;
	case Xavi::Token::ERROR:
		std::cout << "Error";
		break;
	default:
		std::cout << "Character(" << (char)MyToken.Type() << ")" << std::endl;
	}
}

class DataSourceStub : public Xavi::DataSource
{
public:
	DataSourceStub();
	virtual ~DataSourceStub();
	virtual bool Advance();
	virtual char GetCurrent();

private:
	const char *input;
	int position;
};

DataSourceStub::DataSourceStub()
	: input("abs(2) + 300 / 3d6 + 5.25 * sin(200) - 5"),
	position(0)
{
}

bool DataSourceStub::Advance()
{
	if (input[position])
	{
		position++;
		return true;
	}
	else
	{
		return false;
	}
}

char DataSourceStub::GetCurrent()
{
	return input[position];
}

DataSourceStub::~DataSourceStub()
{
}

int main(int argc, char *argv[])
{
	Xavi::Lexer MyLexer(new DataSourceStub());

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::ID
		|| std::strcmp(MyLexer.GetToken().Id(), "abs") != 0)
	{
		std::cout << "Failed, expecting: ID(abs)" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::LPAREN)
	{
		std::cout << "Failed, expecting: (" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 2)
	{
		std::cout << "Failed, expecting: Integer: 2" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::RPAREN)
	{
		std::cout << "Failed, expecting: )" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::ADDITION)
	{
		std::cout << "Failed, expecting: +" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 300)
	{
		std::cout << "Failed, expecting: Integer: 300" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::DIVISION)
	{
		std::cout << "Failed, expecting: /" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 3)
	{
		std::cout << "Failed, expecting: Integer: 3" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::DICE)
	{
		std::cout << "Failed, expecting: d" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 6)
	{
		std::cout << "Failed, expecting: Integer: 6" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::ADDITION)
	{
		std::cout << "Failed, expecting: +" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::FLOAT
		|| MyLexer.GetToken().Float() != 5.25)
	{
		std::cout << "Failed, expecting: Integer: 5.25" << std::endl;
		return 1;
	}
	MyLexer.Next();

	//Character(*)
	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::MULTIPLY)
	{
		std::cout << "Failed, expecting: *" << std::endl;
		return 1;
	}
	MyLexer.Next();
	//ID(sin)
	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::ID
		|| strcmp(MyLexer.GetToken().Id(), "sin") != 0)
	{
		std::cout << "Failed, expecting: ID(sin)" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::LPAREN)
	{
		std::cout << "Failed, expecting: (" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 200)
	{
		std::cout << "Failed, expecting: Integer: 200" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::RPAREN)
	{
		std::cout << "Failed, expecting: )" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::SUBTRACT)
	{
		std::cout << "Failed, expecting: -" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::INTEGER
		|| MyLexer.GetToken().Integer() != 5)
	{
		std::cout << "Failed, expecting: Integer: 5" << std::endl;
		return 1;
	}
	MyLexer.Next();

	PrintToken(MyLexer.GetToken());
	if (MyLexer.GetToken().Type() != Xavi::Token::EOL)
	{
		std::cout << "Failed, expecting: EOL" << std::endl;
		return 1;
	}

	std::cout << "Total Success!" << std::endl;
}
