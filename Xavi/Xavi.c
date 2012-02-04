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

#include "Xavi.h"
#include "XaviFunctionCall.h"
#include "XaviOperatorCall.h"
#include "XaviYyParser.h"
#include "XaviYyLexer.h"

int Xavi_yyparse(XaviNumber *, yyscan_t);

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
	yyscan_t scannerState;
	YY_BUFFER_STATE buffer;

	buffer = Xavi_yy_scan_string(inString, scannerState);
	Xavi_yy_switch_to_buffer(buffer, scannerState);

	Xavi_yylex_init(&scannerState);
	Xavi_yyparse(&rVal, scannerState);
	Xavi_yylex_destroy(scannerState);

	return rVal;
}
