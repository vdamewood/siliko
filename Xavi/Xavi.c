/*
 * Xavi.c: Public interface for Xavi.
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

#include "Xavi.h"
#include "XaviFunctionCall.h"
#include "XaviParser.h"

void XaviOpen(void)
{
	XaviFunctionCallOpen();
}

void XaviClose(void)
{
	XaviFunctionCallClose();
}

XaviNumber XaviParse(const char * inString)
{
	XaviNumber rVal;
	XaviLexer *lexer;

	lexer = XaviLexerNew(inString);
	rVal = XaviInternalParse(lexer);
	XaviLexerDestroy(&lexer);

	return rVal;
}
