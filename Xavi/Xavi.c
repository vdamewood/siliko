/*
 * Xavi.c: Public interface for Xavi.
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

#include <stdlib.h>

#include "Xavi.h"
#include "XaviFunctionCall.h"
#include "XaviOperatorCall.h"

#if !defined USE_BISON
#define USE_BISON 0
#endif

#if !defined USE_FLEX
#define USE_FLEX 0
#endif

#if USE_FLEX
#if !defined YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void * yyscan_t;
#include "XaviYyParser.h"
#include "XaviYyLexer.h"
#endif
#else
#include "XaviLexer.h"
#endif

#if !USE_BISON
#error "Building without GNU Bison is not yet supported."
#endif /* !USE_BISON */

void XaviOpen(void)
{
	XaviFunctionCallOpen();
	XaviOperatorCallOpen();
}

void XaviClose(void)
{
	XaviFunctionCallClose();
	XaviOperatorCallClose();
}

XaviNumber XaviParse(const char *inString)
{
	XaviNumber rVal;
	XaviMemoryPool pool;

	pool.DanglingTrees = NULL;
	pool.DanglingArglists = NULL;
	pool.DanglingIds = NULL;

#if USE_FLEX
	yyscan_t scannerState;
	YY_BUFFER_STATE buffer;
	Xavi_yylex_init(&scannerState);
	buffer = Xavi_yy_scan_string(inString, scannerState);
	Xavi_yy_switch_to_buffer(buffer, scannerState);
	Xavi_yyparse(&rVal, &pool, scannerState);
	Xavi_yylex_destroy(scannerState);
#else
	XaviLexer *lexer = XaviLexerNew(inString);
	Xavi_yyparse(&rVal, &pool, (void *)lexer);
	XaviLexerDestroy(&lexer);
#endif
	return rVal;
}
