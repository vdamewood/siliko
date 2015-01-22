/*
 * FunctionCaller.hpp: Function handling
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

#if !defined XAVI_FUNCTION_CALL_H
#define XAVI_FUNCTION_CALL_H

#include <list>
#include <vector>

#include "W32Dll.hpp"
#include "Value.hpp"


namespace Xavi
{
	namespace FunctionCaller
	{
		typedef Value (*FunctionPointer)(std::vector<Value>);

		XAVI_API bool Initialize(void);
		XAVI_API void Destroy(void);
		XAVI_API Value Call(const char *Name, std::vector<Value> Args);
		XAVI_API void Install(const char *Name, FunctionPointer Function);
	};
};
#endif // XAVI_FUNCTION_CALL_H
