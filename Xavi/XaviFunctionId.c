/*
 * XaviFunctionId.c: Functions to manipulate function identifiers.
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
#include <string.h>

#include "XaviCleanup.h"
#include "XaviFunctionId.h"
#include "XaviStructs.h"


char * XaviFunctionIdNew(char * NewId, XaviMemoryPool * pool)
{
	char * rVal;
	if (rVal = malloc(strlen(NewId)+1)) {
		XaviCleanupCacheFunctionId(rVal, pool);
		strcpy(rVal, NewId);
	}
	return rVal;
}

void XaviFunctionIdDelete(char * InId)
{
	free(InId);
}
