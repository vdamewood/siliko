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

#include "XaviLexer.h"

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

	XaviLexer *lexer = XaviLexerNew(inString);
	Xavi_yyparse(&rVal, &pool, (void *)lexer);
	XaviLexerDestroy(&lexer);

	return rVal;
}
