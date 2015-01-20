/*
 * Functions.cpp: Built-in functions.
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
#include <limits>

#include "Functions.hpp"

Xavi::Value Xavi::Functions::add(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::subtract(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::multiply(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::divide(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::power(std::vector<Xavi::Value> Args)
{
	float runningValue;

	runningValue = Args[0].FloatValue();

	for (std::vector<Xavi::Value>::iterator i = Args.begin(); i != Args.end(); i++)
		runningValue = std::pow(runningValue, i->FloatValue());
	return runningValue;
}


Xavi::Value Xavi::Functions::dice(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::abs(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::acos(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < -1 || Args[0].FloatValue() > 1)
		return Xavi::Value::DOMAIN_ERR;

	return std::acos(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::asin(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < -1 || Args[0].FloatValue() > 1)
		return Xavi::Value::DOMAIN_ERR;

	return std::asin(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::atan(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::atan(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::ceil(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::cos(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::cos(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::cosh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::cosh(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::exp(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::exp(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::floor(std::vector<Xavi::Value> Args)
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

Xavi::Value Xavi::Functions::log(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::log(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::log10(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::log10(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::sin(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::sin(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::sinh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::sinh(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::sqrt(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	if (Args[0].FloatValue() < 0.0)
		return Xavi::Value::DOMAIN_ERR;

	return std::sqrt(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::tan(std::vector<Xavi::Value> Args)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::tan(Args[0].FloatValue());
}

Xavi::Value Xavi::Functions::tanh(std::vector<Xavi::Value> Args)
{
	if (Args.size() != 1)
		return Xavi::Value::BAD_ARGUMENTS;

	return std::tanh(Args[0].FloatValue());
}

