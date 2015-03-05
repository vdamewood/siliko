/*
 * Value.cpp: Data structure to represent a single value
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

#include <cstring>
#include <sstream>
#include <limits>

#include "Value.hpp"

Xavi::Value::Value()
{
	MyStatus = INTEGER;
	MyInteger = 0;
}

Xavi::Value::Value(Xavi::Value::ValueStatus NewStatus)
{
	MyStatus = NewStatus;
	MyInteger = 0;
}

Xavi::Value::Value(int NewValue)
{
	MyStatus = INTEGER;
	MyInteger = NewValue;
}

Xavi::Value::Value(float NewValue)
{
	MyStatus = FLOAT;
	MyFloat = NewValue;
}

Xavi::Value::ValueStatus Xavi::Value::Status(void) const
{
	return MyStatus;
}

int Xavi::Value::Integer() const
{
	if (MyStatus == INTEGER)
		return MyInteger;
	else if (MyStatus == FLOAT)
		return static_cast<int>(MyFloat);
	else
		return 0;
}

float Xavi::Value::Float() const
{
	if (MyStatus == INTEGER)
		return static_cast<float>(MyInteger);
	else if (MyStatus == FLOAT)
		return MyFloat;
	else
		return std::numeric_limits<float>::quiet_NaN();
}

bool Xavi::Value::IsNumber() const
{
	return (MyStatus == INTEGER || MyStatus == FLOAT);
}

char *Xavi::Value::ToCString() const
{
	std::ostringstream tmp;
	switch (MyStatus)
	{
	case INTEGER:
		tmp << Integer();
		break;
	case FLOAT:
		tmp << Float();
		break;
	case MEMORY_ERR:
		tmp << "Error: Out of memory";
		break;
	case SYNTAX_ERR:
		tmp << "Error: Syntax error";
		break;
	case ZERO_DIV_ERR:
		tmp << "Error: Division by zero";
		break;
	case BAD_FUNCTION:
		tmp << "Error: Function not found";
		break;
	case BAD_ARGUMENTS:
		tmp << "Error: Bad argument count";
		break;
	case DOMAIN_ERR:
		tmp << "Error: Domain error";
		break;
	case RANGE_ERR:
		tmp << "Error: Range error";
	}

	const std::string &tmpString = tmp.str();
	char * rValue = new char[tmpString.size()+1];
	std::strcpy(rValue, tmpString.c_str());
	return rValue;
}
