/*
 * XaviFunctionCall.h: Function handling.
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

#if !defined Xavi_FUNCTION_CALL_H
#define Xavi_FUNCTION_CALL_H

#include <list>
#include <string>
#include <vector>

#include "Value.hpp"

namespace Xavi
{
	class FunctionCaller
	{
	public:
		typedef Value (*FunctionPointer)(std::vector<Value>);

		static FunctionCaller &Default(void);
		static void DeleteDefault(void);
		static unsigned char Hash(const unsigned char *rawInput, size_t length);

		FunctionCaller(void);
		~FunctionCaller(void);

		Value Call(std::string Name, std::vector<Value> Args);
		void Install(std::string Name, FunctionPointer Function);
		void InstallBuiltins(void);
	private:
		static FunctionCaller *DefaultInstance;

		class LookupNode
		{
		public:
			LookupNode(std::string NewId, FunctionPointer NewFunction);
			~LookupNode(void);
			std::string id;
			FunctionPointer function;
		};
		std::vector<std::list<LookupNode> > lookup;
	};
};
#endif // Xavi_FUNCTION_CALL_H
