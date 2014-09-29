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

namespace Xavi
{
	class FunctionChain
	{
	public:
		FunctionChain(std::string NewId, Xavi::FunctionCaller::Pointer NewFunction);
		~FunctionChain(void);
		std::string id;
		Xavi::FunctionCaller::Pointer function;
		FunctionChain *next;
	};
};
static Xavi::FunctionChain **functionTable = 0;

Xavi::FunctionChain::FunctionChain(std::string NewId, Xavi::FunctionCaller::Pointer NewFunction)
{
	id = NewId;
	function = NewFunction;
	next = NULL;
}

Xavi::FunctionChain::~FunctionChain(void)
{
	if (next)
		delete next;
}

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

bool Xavi::FunctionCaller::Install(std::string Name, Xavi::FunctionCaller::Pointer Function)
{
	int index = Xavi::FunctionCaller::Hash((const unsigned char *)Name.c_str(), Name.size());
	Xavi::FunctionChain *NewEntry = new Xavi::FunctionChain(Name, Function);

	if (functionTable)
	{
		if (functionTable[index])
		{
			Xavi::FunctionChain *currentChain = functionTable[index];
			while (currentChain->next)
				currentChain = currentChain->next;
			currentChain->next = NewEntry;
		}
		else
		{
			functionTable[index] = NewEntry;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Xavi::FunctionCaller::Open()
{
	functionTable = new Xavi::FunctionChain*[256];

	for (int i = 0; i<=255; i++)
		functionTable[i] = NULL;

	Install("add", Xavi::Functions::add);
	Install("subtract", Xavi::Functions::subtract);
	Install("multiply", Xavi::Functions::multiply);
	Install("divide", Xavi::Functions::divide);
	Install("power", Xavi::Functions::power);
	Install("dice", Xavi::Functions::dice);

	Install("abs", Xavi::Functions::abs);
	Install("acos", Xavi::Functions::acos);
	Install("asin", Xavi::Functions::asin);
	Install("atan", Xavi::Functions::atan);
	Install("ceil", Xavi::Functions::ceil);
	Install("cos", Xavi::Functions::cos);
	Install("cosh", Xavi::Functions::cosh);
	Install("exp", Xavi::Functions::exp);
	Install("floor", Xavi::Functions::floor);
	Install("log", Xavi::Functions::log);
	Install("log10", Xavi::Functions::log10);
	Install("sin", Xavi::Functions::sin);
	Install("sinh", Xavi::Functions::sinh);
	Install("sqrt", Xavi::Functions::sqrt);
	Install("tan", Xavi::Functions::tan);
	Install("tanh", Xavi::Functions::tanh);

	return true;
}

void Xavi::FunctionCaller::Close()
{
	if (functionTable)
	{
		for (int i = 0; i <= 255; i++)
			if (functionTable[i])
				delete functionTable[i];

		delete[] functionTable;
	}
}

Xavi::Value Xavi::FunctionCaller::Call(std::string Name, std::vector<Xavi::Value> Args)
{
	int index = Xavi::FunctionCaller::Hash((const unsigned char *)Name.c_str(), Name.size());
	Xavi::FunctionChain *current = functionTable[index];

	while (current && Name != current->id)
		current = current->next;

	if (current)
		return current->function(Args);
	else
		return Xavi::Value::BAD_FUNCTION;
}
