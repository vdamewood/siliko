/*
 * XaviTestFunctions.cpp: Program to test Xavi's function-calling code.
 * Copyright 2012, 2014 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "Xavi++/FunctionCaller.hpp"

int main()
{
	std::vector<Xavi::Value> Inputs; // Arguments to functions.
	Xavi::Value *InputValue = 0; // Used to add values to Inputs.
	Xavi::Value Result; // Return value of function.
	int Total = 0;    // Total number of tests run.
	int Success = 0;  // Number of successful tests.

	Xavi::FunctionCaller::Initialize();

	////////////////////////////////////////////////////////////////////

	std::cout << "Test 1: abs(-400) returns 400... ";

	//InputValue = new Xavi::Value(400);
	Inputs.push_back(Xavi::Value(400));
	//Inputs.push_back(*InputValue);
	//delete InputValue;
	Total++;
	Result = Xavi::FunctionCaller::Call("abs", Inputs);
	if (Result.Status() == Xavi::Value::INTEGER)
	{
		if (Result.IntegerValue() == 400)
		{
			std::cout << "success." << std::endl;
			Success++;
		}
		else
		{
			std::cout << "failed. Value returned: "
				<< Result.IntegerValue() << std::endl;
		}
	}
	else if (Result.Status() == Xavi::Value::FLOAT)
	{
		std::cout << "failed. Value returned: "
			<< Result.FloatValue() << std::endl;
	}
	else
	{
		std::cout << "failed. Error code returned: "
			<< Result.Status() << std::endl;
	}

	////////////////////////////////////////////////////////////////////

	Xavi::FunctionCaller::Destroy();

	// Summary
	std::cout << "Tests passed/run: " << Success << "/" << Total << std::endl
		<< "Percentage: " << (((float) Success / (float) Total) * 100.00)
		<< "%" << std::endl;
	return Total - Success;
}
