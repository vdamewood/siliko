/*
 * XvcFunctionId.c: Functions to manipulate function identifiers.
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

#include <stdlib.h>
#include <string.h>

#include "XvcCleanup.h"
#include "XvcFunctionId.h"
#include "XvcStructs.h"


char * XvcFunctionIdNew(char * NewId)
{
	char * rVal;
	rVal = malloc(strlen(NewId)+1);
	XvcCleanupCacheFunctionId(rVal);
	strcpy(rVal, NewId);
	return rVal;
}

void XvcFunctionIdDelete(char * InId)
{
	XvcCleanupReleaseFunctionId(InId);
	free(InId);
}
