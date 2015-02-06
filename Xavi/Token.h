/*
 * Token.h: Token used by lexical analyzer
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

#if !defined XAVI_TOKEN_H
#define XAVI_TOKEN_H

enum XAVI_API XaviTokenType
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
	DICE = 'd',
	INTEGER = 256,
	FLOAT,
	ID,
	EOL
};
typedef enum XaviTokenType XaviTokenType;

struct XAVI_API XaviToken
{
	XaviTokenType Type;
	union
	{
		char * String;
		int Integer;
		float Float;
	};
};
typedef struct XaviToken XaviToken;

#endif /* XAVI_TOKEN_H */
