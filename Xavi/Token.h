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

#if defined __cplusplus
extern "C" {
#endif

enum XAVI_API XaviTokenType
{
	XAVI_TOK_ERROR = -1,
	XAVI_TOK_UNSET = 0,
	XAVI_TOK_LPAREN = '(',
	XAVI_TOK_RPAREN = ')',
	XAVI_TOK_MULTIPLY = '*',
	XAVI_TOK_ADDITION = '+',
	XAVI_TOK_COMMA = ',',
	XAVI_TOK_SUBTRACT = '-',
	XAVI_TOK_DIVISION = '/',
	XAVI_TOK_EXPONENT = '^',
	XAVI_TOK_DICE = 'd',
	XAVI_TOK_INTEGER = 256,
	XAVI_TOK_FLOAT,
	XAVI_TOK_ID,
	XAVI_TOK_EOL
};
typedef enum XaviTokenType XaviTokenType;

struct XAVI_API XaviToken
{
	XaviTokenType Type;
	union
	{
		char *Id;
		int Integer;
		float Float;
	};
};
typedef struct XaviToken XaviToken;

#if defined __cplusplus
}
#endif

#endif /* XAVI_TOKEN_H */
