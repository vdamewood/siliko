/*
 * FunctionCaller.cpp: Function handling
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

#include <unordered_map>

#include "FunctionCaller.hpp"
#include "Functions.hpp"

// This is initialized as a null pointer rather than a static object so that
// bindings can be created for language interpreters written in C.
std::unordered_map<std::string, Xavi::FunctionCaller::FunctionPointer> *lookup = 0;

bool Xavi::FunctionCaller::SetUp(void)
{
	try
	{
		lookup = new std::unordered_map<std::string,
			Xavi::FunctionCaller::FunctionPointer>();
	}
	catch (...)
	{
		lookup = 0;
		return false;
	}

	// Built-in operators
	Install("add", Xavi::Functions::add);
	Install("subtract", Xavi::Functions::subtract);
	Install("multiply", Xavi::Functions::multiply);
	Install("divide", Xavi::Functions::divide);
	Install("power", Xavi::Functions::power);
	Install("dice", Xavi::Functions::dice);

	// Math library functions
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

void Xavi::FunctionCaller::TearDown(void)
{
	delete lookup;
	lookup = 0;
}

void Xavi::FunctionCaller::Install(const std::string &Name, FunctionPointer Function)
{
	(*lookup)[Name] = Function;
}

Xavi::Value Xavi::FunctionCaller::Call(const std::string &Name, std::vector<Xavi::Value> Args)
try
{
	return lookup->at(Name)(Args);
}
catch (const std::out_of_range &)
{
	return Xavi::Value::BAD_FUNCTION;
}
