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
		enum TokenType
		{
			ERROR = -1,
			INTEGER = 0,
			FLOAT,
			OPERATOR,
			ID,
			EOL
		};

		Token(TokenType);
		Token(int IntegerValue);
		Token(float FloatValue);
		Token(char OperatorValue);
		Token(const std::string &IdValue);
		Token(const char *IdValue);
		~Token(void);

		TokenType   GetType(void);
		int         GetIntegerValue(void);
		float       GetFloatValue(void);
		char        GetOperatorValue(void);
		std::string GetIdValue(void);

	private:
		TokenType Type;
		union
		{
			int IntegerValue;
			float FloatValue;
			char OperatorValue;
			std::string *IdValue;
		};
	};
};

#endif /* XAVI_TOKEN_H */
