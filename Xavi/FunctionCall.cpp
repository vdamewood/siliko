/*
 * XaviFunctionCall.c: Function handling.
 * Copyright 2012, 2014 Vincent Damewood
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

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <vector>

#include "Functions.hpp"
#include "FunctionCall.hpp"
#include "Value.hpp"

unsigned char Xavi::FunctionCaller::Hash(const unsigned char *rawInput, size_t length)
{
	const unsigned char divisor = 0xD5;
	unsigned char result = 0x00;
	unsigned int i;
	int j;
	int hasHighBit;

	for (i = 0; i <= length; i++)
	{
		for (j = 7; j >= 0; j--)
		{
			hasHighBit = result & 0x80;
			result <<= 1;

			result |= (i < length)
				? (rawInput[i] >> j) & 0x01
				: 0;

			if (hasHighBit)
				result ^= divisor;
		}
	}

	return result;
}

typedef Xavi::Value (*FunctionPointer)(std::vector<Xavi::Value>);

class XaviFunctionChain
{
public:
	std::string id;
	FunctionPointer function;
	XaviFunctionChain *next;
};

static XaviFunctionChain **functionTable;

bool Xavi::FunctionCaller::Open()
{
	const std::string functionNames[] =
	{
		"add", "subtract", "multiply", "divide",
		"power", "dice",
		"abs", "acos", "asin", "atan",
		"ceil", "cos", "cosh", "exp",
		"floor", "log","log10", "sin",
		"sinh", "sqrt", "tan", "tanh"
	};

	const FunctionPointer functions[] =
	{
		Xavi::Functions::add, Xavi::Functions::subtract, Xavi::Functions::multiply, Xavi::Functions::divide,
		Xavi::Functions::power, Xavi::Functions::dice,
		Xavi::Functions::abs, Xavi::Functions::acos, Xavi::Functions::asin, Xavi::Functions::atan,
		Xavi::Functions::ceil, Xavi::Functions::cos, Xavi::Functions::cosh, Xavi::Functions::exp,
		Xavi::Functions::floor, Xavi::Functions::log, Xavi::Functions::log10, Xavi::Functions::sin,
		Xavi::Functions::sinh, Xavi::Functions::sqrt, Xavi::Functions::tan, Xavi::Functions::tanh	};

	const int FUNCTION_COUNT = 22;
	const int FUNCTION_MAX = FUNCTION_COUNT - 1;

	functionTable = new XaviFunctionChain*[256];
	XaviFunctionChain **tempTable = new XaviFunctionChain*[FUNCTION_COUNT];

	for (int i = 0; i<=FUNCTION_MAX; i++)
	{
		tempTable[i] = new XaviFunctionChain();
	}

	for (int i = 0; i<=255; i++)
		functionTable[i] = NULL;

	for (int i = 0; i <= FUNCTION_MAX; i++)
	{
		int index = Hash((const unsigned char *)functionNames[i].c_str(), functionNames[i].size());

		tempTable[i]->id = functionNames[i];
		tempTable[i]->function = functions[i];
		tempTable[i]->next = NULL;

		if (functionTable[index])
		{
			XaviFunctionChain *currentChain = functionTable[index];
			while (currentChain->next)
				currentChain = currentChain->next;
			currentChain->next = tempTable[i];
		}
		else
		{
			functionTable[index] = tempTable[i];
		}
	}

	delete[] tempTable;
	return true;
}

#include <iostream>

void Xavi::FunctionCaller::Close()
{
	if (functionTable)
	{
		for (int i = 0; i <= 255; i++)
			if (functionTable[i])
			{
				XaviFunctionChain *current = functionTable[i];
				while (current)
				{
					XaviFunctionChain *next = current->next;
					delete current;
					current = next;
				}
			}

		delete[] functionTable;
		functionTable = NULL;
	}
}

static FunctionPointer GetFunction(std::string Name)
{
	int index;
	XaviFunctionChain *current;

	index = Xavi::FunctionCaller::Hash((const unsigned char *)Name.c_str(), Name.size());
	current = functionTable[index];

	while (current)
		if (Name != current->id)
			current = current->next;
		else
			break;

	if (current)
		return current->function;
	else
		return NULL;
}

Xavi::Value Xavi::FunctionCaller::Call(std::string Name, std::vector<Xavi::Value> Args)
{
	FunctionPointer Function;

	Function = GetFunction(Name);

	if (!Function)
		return Xavi::Value::BAD_FUNCTION;
	else
		return Function(Args);
}
