/*
 * Lexer.h: Lexical analyzer
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

#if !defined XAVI_LEXER_H
#define XAVI_LEXER_H

#include "W32Dll.h"
#include "DataSource.h"
#include "Token.h"

#if defined __cplusplus
extern "C" {
#endif

struct XAVI_API XaviLexer
{
	XaviDataSource *Source;
	XaviToken Token;
};
typedef struct XaviLexer XaviLexer;

XAVI_API XaviLexer *XaviLexerNew(XaviDataSource *input);
XAVI_API void XaviLexerDelete(XaviLexer *lexer);

XAVI_API void XaviLexerNext(XaviLexer *lexer);

#if defined __cplusplus
}
#endif

#endif /* XAVI_LEXER_H */
