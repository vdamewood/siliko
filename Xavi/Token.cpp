/*
 * Token.cpp: Token passed from tokenizer to parser.
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

// Visual Studio complains about strcpy without this.
#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <cstring>

#include "Token.hpp"

Xavi::Token::Token(Type NewType)
{
	MyType = NewType;
	IntegerValue = 0;
}

Xavi::Token::Token(int NewIntegerValue)
{
	MyType = INTEGER;
	IntegerValue = NewIntegerValue;
}

Xavi::Token::Token(float NewFloatValue)
{
	MyType = FLOAT;
	FloatValue = NewFloatValue;
}

Xavi::Token::Token(const char *NewIdValue)
{
	MyType = ID;
	IdValue = new char[std::strlen(NewIdValue) + 1];
	std::strcpy(IdValue, NewIdValue);
}


Xavi::Token::~Token()
{
	if (MyType == ID)
		delete[] IdValue;
}

Xavi::Token::Type Xavi::Token::GetType(void)
{
	return MyType;
}

int Xavi::Token::GetIntegerValue(void)
{
	return IntegerValue;
}

float Xavi::Token::GetFloatValue(void)
{
	return FloatValue;
}

const char *Xavi::Token::GetIdValue(void)
{
	return IdValue;
}
