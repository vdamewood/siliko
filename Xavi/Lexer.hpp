/*
 * XaviLexer.h: Lexical analyzer
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

#if !defined XAVI_LEXER_H
#define XAVI_LEXER_H

#include <string>

namespace Xavi
{
	// FIXME: Separate Token into its own type.
	enum TokenType
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

	union TokenValue
	{
		char * s;
		int i;
		float f;
	};

	class Lexer
	{
	public:
		Lexer(const char *InputString);
		~Lexer();

		TokenType GetToken(void);
		TokenValue GetValue(void);
		void Next(void);

	private:
		void Load(void);
		const char *location;
		TokenType token;
		TokenValue value;
	};
};

#endif /* XAVI_LEXER_H */
