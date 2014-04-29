/*
 * XaviParser.c: Parser for Xavi.
 * Copyright 2014 Vincent Damewood
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
#include "XaviStructs.h"
#include "XaviParser.h"

#include "XaviCleanup.h"
#include "XaviEvaluate.h"
#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviFunctionId.h"


static void XaviParseError(
	XaviNumber * result,
	XaviMemoryPool * pool)
{
        result->status = E_SYNTAX;
        XaviCleanupClearAll(pool);
}


static XaviTree * XaviGetInteger(XaviLexer * lexer, XaviMemoryPool * pool)
{
	XaviTokenValue value;
	int token = XaviLexerRead(lexer, &value);


	if (token == INTEGER)
	{
		XaviTree * rVal = XaviTreeNewInteger(value.i, pool);
		return rVal;
	}
	else
	{
		return NULL;
	}
}

int XaviInternalParse(
	XaviNumber * value,
	XaviMemoryPool * pool,
	XaviLexer * lexer)
{
	// FIXME: Change GetInteger to GetCalculation
	XaviTree * tree = XaviGetInteger(lexer, pool);

	if (tree != NULL)
	{
		*value = XaviEvaluate(tree);
        	XaviCleanupClearAll(pool);
	}
	else
	{
		value->status = E_SYNTAX;
		value->i = 0;
	}

	return 0;
}
