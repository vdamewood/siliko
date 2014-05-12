/*
 * XaviArglist.c: Functions to manipulate arguments lists for functions.
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

#include <stdlib.h>

#include "XaviArglist.h"
#include "XaviTree.h"

XaviArglist *XaviArglistNew(XaviTree *NewTree, XaviArglist *OldList)
{
	XaviArglist *rVal;

	if ((rVal = malloc(sizeof(XaviArglist))))
	{
		if (OldList)
			rVal->depth = OldList->depth + 1;
		else
			rVal->depth = 1;
		rVal->value = NewTree;
		rVal->next = OldList;
	}
	return rVal;
}

void XaviArglistDelete(XaviArglist *OldArglist)
{
	if (OldArglist)
	{
		XaviArglistDelete(OldArglist->next);
		XaviTreeDelete(OldArglist->value);
		free(OldArglist);
	 }
}

XaviTree **XaviArglistGetTrees(XaviArglist *InArglist)
{
	XaviTree **rVal;
	XaviArglist *Current = InArglist;
	XaviArglist *next;
	int i;
	int depth = InArglist->depth;

	if ((rVal = malloc(depth * sizeof(XaviTree *))))
	{
		for (i = 0; i < depth; i++)
		{
			rVal[i] = Current->value;
			next = Current->next;
			free(Current);
			Current = next;
		}
	}
	else
	{
		rVal = NULL;
	}

	return rVal;
}
