/*
 * XaviLexer.h: Hand-written lexical analyzer.
 * Copyright 2012 Vincent Damewood
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

enum XaviToken
{
	UNSET = -1,
	EOL = 0,
	LPAREN = '(',
	RPAREN = ')',
	MULTIPLY = '*',
	ADDITION = '+',
	COMMA = ',',
	SUBTRACT = '-',
	DIVISION = '/',
	EXPONENT = '^',
	INTEGER = 258,
	FLOAT = 259,
	ID = 260,
	ERROR = 261
};
typedef enum XaviToken XaviToken;

union XaviTokenValue
{
        char * s;
        int i;
        float f;
};
typedef union XaviTokenValue XaviTokenValue;

struct XaviLexer
{
	const char * input;
	const char * location;
	XaviToken token;
	XaviTokenValue value;
};
typedef struct XaviLexer XaviLexer;

XaviLexer * XaviLexerNew(const char * inputString);
void XaviLexerDestroy(XaviLexer** lexer);

XaviToken XaviLexerGetToken(XaviLexer * lexer);
XaviTokenValue XaviLexerGetValue(XaviLexer * lexer);
void XaviLexerNext(XaviLexer * lexer);
#endif /* XAVI_LEXER_H */
