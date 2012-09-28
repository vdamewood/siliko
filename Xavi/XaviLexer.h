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

#if !defined USE_BISON
#define USE_BISON 0
#endif /* USE_BISON */

struct XaviLexer
{
	const char * input;
	const char * begin;
	const char * current;
	char * lexeme;
};

typedef struct XaviLexer XaviLexer;
typedef XaviLexer * yyscan_t;

#if USE_BISON
#include "XaviYyParser.h"
#else
#error "Building without BISON is currently not supported."
#endif


int XaviLexerRead(XaviLexer *, YYSTYPE *);
void XaviLexerDestroy(XaviLexer**);
XaviLexer * XaviLexerNew(const char * inputString);
#define Xavi_yylex(a,b) XaviLexerRead((b), (a))
#endif
