/*
 * Value.hpp: Data structure to represent a single value
 * Copyright 2014, 2015 Vincent Damewood
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

#if !defined XAVI_VALUE_HPP
#define XAVI_VALUE_HPP

#include "W32Dll.hpp"

namespace Xavi
{
	class XAVI_API Value
	{
	public:
		enum XAVI_API ValueStatus
		{
			INTEGER,       /* Success: The value is an integer. */
			FLOAT,         /* Success: The valis is a float. */
			MEMORY_ERR,    /* Error: An attempt to allocate memory
			                    failed. */
			SYNTAX_ERR,    /* Error: A syntax error was
			                    encountered. */
			ZERO_DIV_ERR,  /* Error: Division by zero was
			                    attempted. */
			BAD_FUNCTION,  /* Error: A function call could not be
			                    resolved to a valid function. */
			BAD_ARGUMENTS, /* Error: A function call was made with
			                    a number of arguments that the
			                    function can't accept. */
			DOMAIN_ERR,    /* Error: A function call resulted in a
			                    domain error. */
			RANGE_ERR      /* Error: A function call resulted in a
			                    range error. */
		};

		Value();
		Value(ValueStatus NewStatus);
		Value(int NewIntegerValue);
		Value(float NewFloatValue);

		ValueStatus Status(void) const;
		int Integer(void) const;
		float Float(void) const;

		bool IsNumber(void) const;
	private:
		ValueStatus MyStatus;
		union
		{
			int MyInteger;
			float MyFloat;
		};
	};
}

#endif // XAVI_VALUE_HPP
