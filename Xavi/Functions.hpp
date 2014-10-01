/*
 * Functions.hpp: Built-in functions
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

#if !defined Xavi_FUNCTIONS_H
#define Xavi_FUNCTIONS_H

#include <vector>

#include "Value.hpp"

#if !defined API
#  if defined _WIN32 && defined _MSC_VER
#    if defined Xavi_EXPORTS
#      define API
#    else Xavi_EXPORTS
#      define API __declspec(dllimport)
#    endif // Xavi_EXPORTS
#  else // _WIN32 && __MSC_VER
#    define API
#  endif // _WIN32 && __MSC_VER
#endif // API

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
#endif // Xavi_FUNCTIONS_H
