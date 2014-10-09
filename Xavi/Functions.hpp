/*
 * Functions.hpp: Built-in functions.
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

#if !defined XAVI_FUNCTIONS_H
#define XAVI_FUNCTIONS_H

#include <vector>

#include "W32Dll.hpp"
#include "Value.hpp"

namespace Xavi
{
	namespace Functions
	{
		Xavi::Value add(std::vector<Xavi::Value> Args);
		Xavi::Value subtract(std::vector<Xavi::Value> Args);
		Xavi::Value multiply(std::vector<Xavi::Value> Args);
		Xavi::Value divide(std::vector<Xavi::Value> Args);
		Xavi::Value power(std::vector<Xavi::Value> Args);
		Xavi::Value dice(std::vector<Xavi::Value> Args);
		Xavi::Value abs(std::vector<Xavi::Value> Args);
		Xavi::Value acos(std::vector<Xavi::Value> Args);
		Xavi::Value asin(std::vector<Xavi::Value> Args);
		Xavi::Value atan(std::vector<Xavi::Value> Args);
		Xavi::Value ceil(std::vector<Xavi::Value> Args);
		Xavi::Value cos(std::vector<Xavi::Value> Args);
		Xavi::Value cosh(std::vector<Xavi::Value> Args);
		Xavi::Value exp(std::vector<Xavi::Value> Args);
		Xavi::Value floor(std::vector<Xavi::Value> Args);
		Xavi::Value log(std::vector<Xavi::Value> Args);
		Xavi::Value log10(std::vector<Xavi::Value> Args);
		Xavi::Value sin(std::vector<Xavi::Value> Args);
		Xavi::Value sinh(std::vector<Xavi::Value> Args);
		Xavi::Value sqrt(std::vector<Xavi::Value> Args);
		Xavi::Value tan(std::vector<Xavi::Value> Args);;
		Xavi::Value tanh(std::vector<Xavi::Value> Args);
	};
};
#endif // XAVI_FUNCTIONS_H
