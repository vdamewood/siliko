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

#include "FunctionCall.hpp"
#include "Functions.hpp"

namespace Xavi
{
	namespace FunctionCaller
	{
		class LookupNode
		{
		public:
			LookupNode(std::string NewId, FunctionPointer NewFunction);
			~LookupNode(void);
			std::string id;
			FunctionPointer function;
		};
	};
};

static std::vector< std::list<Xavi::FunctionCaller::LookupNode> > *lookup = 0;

static unsigned char Hash(const unsigned char *rawInput, size_t length)
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

bool Xavi::FunctionCaller::Initialize(void)
{
	try
	{
		lookup = new std::vector<std::list<Xavi::FunctionCaller::LookupNode> >(256);
	}
	catch (...)
	{
		lookup = 0;
		return false;
	}

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

void Xavi::FunctionCaller::Destroy(void)
{
	delete lookup;
	lookup = 0;
}

void Xavi::FunctionCaller::Install(std::string Name, FunctionPointer Function)
{
	(*lookup)[Hash((const unsigned char *)Name.c_str(), Name.size())].push_back(LookupNode(Name, Function));
}

Xavi::Value Xavi::FunctionCaller::Call(std::string Name, std::vector<Xavi::Value> Args)
{
	int index = Hash((const unsigned char *)Name.c_str(), Name.size());

	for
		(
			std::list<Xavi::FunctionCaller::LookupNode>::iterator i = (*lookup)[index].begin();
			i != (*lookup)[index].end();
			i++
		)
		{
			if (Name == i->id)
				return i->function(Args);
		}

		return Xavi::Value::BAD_FUNCTION;
}

Xavi::FunctionCaller::LookupNode::LookupNode(std::string NewId, FunctionPointer NewFunction)
{
	id = NewId;
	function = NewFunction;
}

Xavi::FunctionCaller::LookupNode::~LookupNode(void)
{
}
