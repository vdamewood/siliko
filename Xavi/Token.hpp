/*
 * XaviToken.hpp: Token passed from lexer to parser
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

#if !defined XAVI_TOKEN_H
#define XAVI_TOKEN_H

#include <string>

namespace Xavi
{
	class Token
	{
	public:
		enum Type
		{
			ERROR = -1,
			UNSET = 0,
			LPAREN = '(',
			RPAREN = ')',
			MULTIPLY = '*',
			ADDITION = '+',
			COMMA = ',',
			SUBTRACT = '-',
			DIVISION = '/',
			EXPONENT = '^',
			INTEGER = 256,
			FLOAT,
			ID,
			EOL
		};

		Token(Type ValuelessType);
		Token(int IntegerValue);
		Token(float FloatValue);
		Token(const std::string &IdValue);
		Token(const char *IdValue);
		~Token(void);

		Type              GetType(void);
		int               GetIntegerValue(void);
		float             GetFloatValue(void);
		const std::string &GetIdValue(void);

	private:
		Type MyType;
		union
		{
			int IntegerValue;
			float FloatValue;
			std::string *IdValue;
		};
	};
};

#endif /* XAVI_TOKEN_H */
