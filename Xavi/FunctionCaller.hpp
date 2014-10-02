/*
 * FunctionCaller.hpp: Function handling.
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

#if !defined XAVI_FUNCTION_CALL_H
#define XAVI_FUNCTION_CALL_H

#include <list>
#include <string>
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
	namespace FunctionCaller
	{
		typedef Value (*FunctionPointer)(std::vector<Value>);

		bool Initialize(void);
		void Destroy(void);
		Value Call(std::string Name, std::vector<Value> Args);
		void Install(std::string Name, FunctionPointer Function);
	};
};
#endif // XAVI_FUNCTION_CALL_H
