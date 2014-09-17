/*
 * XaviToken.cpp: Token passed from lexer to parser
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

#include "Token.hpp"

Xavi::Token::Token(TokenType NewType)
{
	Type = NewType;
	IntegerValue = 0;
}

Xavi::Token::Token(int NewIntegerValue)
{
	Type = INTEGER;
	IntegerValue = NewIntegerValue;
}

Xavi::Token::Token(float NewFloatValue)
{
	Type = FLOAT;
	IntegerValue = NewFloatValue;
}

Xavi::Token::Token(char NewOperatorValue)
{
	Type = OPERATOR;
	OperatorValue = NewOperatorValue;
}

Xavi::Token::Token(const std::string &NewIdValue)
{
	Type = ID;
	IdValue = new std::string(NewIdValue);
}

Xavi::Token::Token(const char *NewIdValue)
{
	Type = ID;
	IdValue = new std::string(NewIdValue);
}


Xavi::Token::~Token()
{
	if (Type == ID)
		delete IdValue;
}
