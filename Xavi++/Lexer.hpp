/*
 * Lexer.hpp: Lexical analyzer
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

#if !defined XAVI_LEXER_HPP
#define XAVI_LEXER_HPP

#include "W32Dll.hpp"
#include "DataSource.hpp"
#include "Token.hpp"

namespace Xavi
{
	class XAVI_API Lexer
	{
	public:
		Lexer(DataSource *InputSource);
		~Lexer();

		Xavi::Token &GetToken(void);
		void Next(void);

	private:
		void Load(void);
		DataSource *MySource;
		Xavi::Token *MyToken;
	};
};

#endif // XAVI_LEXER_HPP
