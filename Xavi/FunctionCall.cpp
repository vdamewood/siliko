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

#include "FunctionCall.hpp"
#include "Value.hpp"

static Xavi::Value XaviFunction_add(std::vector<Xavi::Value> Args)
{
	if (!Args.size())
		return 0;

	Xavi::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Xavi::Value::INTEGER)
			rVal = rVal.IntegerValue() + Args[i].IntegerValue();
		else
			rVal = rVal.FloatValue() + Args[i].FloatValue();

	return rVal;
}

static Xavi::Value XaviFunction_subtract(std::vector<Xavi::Value> Args)
{
	if (!Args.size())
		return 0;

	Xavi::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Xavi::Value::INTEGER)
			rVal = rVal.IntegerValue() - Args[i].IntegerValue();
		else
			rVal = rVal.FloatValue() - Args[i].FloatValue();

		return rVal;
}

static Xavi::Value XaviFunction_multiply(std::vector<Xavi::Value> Args)
{
	if (!Args.size())
		return 0;

	Xavi::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Xavi::Value::INTEGER)
			rVal = rVal.IntegerValue() * Args[i].IntegerValue();
		else
			rVal = rVal.FloatValue() * Args[i].FloatValue();

		return rVal;
}

static Xavi::Value XaviFunction_divide(std::vector<Xavi::Value> Args)
{
	if (Args.size() < 2)
		return Xavi::Value::BAD_ARGUMENTS;

	Xavi::Value rVal = Args[0];

	for (int i = 1; i < Args.size(); i++)
	{
		/* Division-by-Zero Error */
		if ((Args[i].Status() == Xavi::Value::FLOAT && Args[i].FloatValue() == 0.0)
			|| (Args[i].Status() == Xavi::Value::INTEGER && Args[i].IntegerValue() == 0))
		{
			return Xavi::Value::ZERO_DIV_ERR;
		}

		if (rVal.Status() == Xavi::Value::FLOAT)
		{
			rVal = rVal.FloatValue() / Args[i].FloatValue();
		}
		else
		{
			if (Args[i].Status() == Xavi::Value::FLOAT
				|| rVal.IntegerValue() % Args[i].IntegerValue() != 0)
			{
				rVal = rVal.FloatValue() / Args[i].FloatValue();
			}
			else
			{
				rVal = rVal.IntegerValue() / Args[i].IntegerValue();
			}
		}
	}
	return rVal;
}

static Xavi::Value XaviFunction_power(std::vector<Xavi::Value> Args)
{
	float runningValue;

	runningValue = Args[0].FloatValue();

	for (std::vector<Xavi::Value>::iterator i = Args.begin(); i != Args.end(); i++)
		runningValue = std::pow(runningValue, i->FloatValue());
	return runningValue;
}


static Xavi::Value XaviFunction_dice(std::vector<Xavi::Value> Args)
{
	/* TODO: Make this function handle fractional dice. */
	static int hasSeeded = 0;

	if(Args.size() != 2)
		return Xavi::Value::BAD_ARGUMENTS;

	if (!hasSeeded)
	{
		hasSeeded = 1;
		std::srand((unsigned int)std::time(NULL));
	}

	int runningTotal = 0;
	for (int i = 1; i <= Args[0].IntegerValue(); i++)
		runningTotal += (std::rand() % Args[1].IntegerValue()) + 1;

	return runningTotal;
}

static Xavi::Value XaviFunction_abs(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	switch (Args[0].Status())
	{
	case Xavi::Value::FLOAT:
		return std::abs(Args[0].FloatValue());
	case Xavi::Value::INTEGER:
		return std::abs(Args[0].IntegerValue());
	default:
		return Args[0];
	}
}

static Xavi::Value XaviFunction_acos(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < -1 || Args[0].FloatValue() > 1)
		return Xavi::Value::DOMAIN_ERR;

	return std::acos(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_asin(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < -1 || Args[0].FloatValue() > 1)
		return Xavi::Value::DOMAIN_ERR;

	return std::asin(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_atan(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::atan(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_ceil(std::vector<Xavi::Value> Args)
{
	float result;

	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	result = std::ceil(Args[0].FloatValue());

	if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
		return static_cast<int>(result);
	else
		return result;
}

static Xavi::Value XaviFunction_cos(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::cos(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_cosh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::cosh(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_exp(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::exp(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_floor(std::vector<Xavi::Value> Args)
{
	float result;

	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	result = std::floor(Args[0].FloatValue());

	if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
		return static_cast<int>(result);
	else
		return result;
}

static Xavi::Value XaviFunction_log(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::log(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_log10(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::log10(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_sin(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::sin(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_sinh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::sinh(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_sqrt(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < 0.0)
		return Xavi::Value::DOMAIN_ERR;

	return std::sqrt(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_tan(std::vector<Xavi::Value> Args)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::tan(Args[0].FloatValue());
}

static Xavi::Value XaviFunction_tanh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::tanh(Args[0].FloatValue());
}

////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////

typedef Xavi::Value (*FunctionPointer)(std::vector<Xavi::Value>);

class XaviFunctionChain
{
public:
	std::string id;
	FunctionPointer function;
	XaviFunctionChain *next;
};

static XaviFunctionChain **functionTable;

bool Xavi::Functions::Open()
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
		XaviFunction_add, XaviFunction_subtract, XaviFunction_multiply, XaviFunction_divide,
		XaviFunction_power, XaviFunction_dice,
		XaviFunction_abs, XaviFunction_acos, XaviFunction_asin, XaviFunction_atan,
		XaviFunction_ceil, XaviFunction_cos, XaviFunction_cosh, XaviFunction_exp,
		XaviFunction_floor, XaviFunction_log, XaviFunction_log10, XaviFunction_sin,
		XaviFunction_sinh, XaviFunction_sqrt, XaviFunction_tan, XaviFunction_tanh
	};

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

void Xavi::Functions::Close()
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

	index = Hash((const unsigned char *)Name.c_str(), Name.size());
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

Xavi::Value Xavi::Functions::Call(std::string Name, std::vector<Xavi::Value> Args)
{
	FunctionPointer Function;

	Function = GetFunction(Name);

	if (!Function)
		return Xavi::Value::BAD_FUNCTION;
	else
		return Function(Args);
}
