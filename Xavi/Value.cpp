/*
 * XaviValue.cpp: Data structure to represent a single value.
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

#include <limits>

#include "Value.hpp"

Xavi::Value::Value(void)
{
	MyStatus = INTEGER;
	MyIntegerValue = 0;
}

Xavi::Value::Value(Xavi::Value::ValueStatus NewStatus)
{
	MyStatus = NewStatus;
	MyIntegerValue = 0;
}

Xavi::Value::Value(int NewValue)
{
	MyStatus = INTEGER;
	MyIntegerValue = NewValue;
}

Xavi::Value::Value(float NewValue)
{
	MyStatus = FLOAT;
	MyFloatValue = NewValue;
}

Xavi::Value::ValueStatus Xavi::Value::Status(void) const
{
	return MyStatus;
}

int Xavi::Value::IntegerValue(void) const
{
	if (MyStatus == INTEGER)
		return MyIntegerValue;
	else if (MyStatus == FLOAT)
		return static_cast<int>(MyFloatValue);
	else
		return 0;
}

float Xavi::Value::FloatValue(void) const
{
	if (MyStatus == INTEGER)
		return static_cast<float>(MyIntegerValue);
	else if (MyStatus == FLOAT)
		return MyFloatValue;
	else
		return std::numeric_limits<double>::quiet_NaN();
}

bool Xavi::Value::IsNumber(void) const
{
	return (MyStatus == INTEGER || MyStatus == FLOAT);
}
