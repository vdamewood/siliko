/*
 * XVCalc.c: Public interface for XVCalc.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */

#include "XVCalc.h"
#include "XvcFunctionCall.h"
#include "XvcOperatorCall.h"
#include "XvcYyLexer.h"

int Xvc_yyparse(XvcNumber *);

void XvcOpen(void)
{
	XvcFunctionCallOpen();
	XvcOperatorCallOpen();
}

void XvcClose(void)
{
	XvcFunctionCallClose();
	XvcOperatorCallClose();
}

XvcNumber XvcParse(const char *inString)
{
	YY_BUFFER_STATE buffer;
	XvcNumber rVal;

	buffer = Xvc_yy_scan_string(inString);
	Xvc_yy_switch_to_buffer(buffer);
	Xvc_yyparse(&rVal);

	return rVal;
}
