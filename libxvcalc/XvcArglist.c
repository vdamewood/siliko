/*
 * XvcArglist.c: Functions to manipulate arguments lists for functions.
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

#include "XvcArglist.h"
#include "XvcCleanup.h"
#include "XvcStructs.h"
#include "XvcTree.h"

XvcArglist * XvcArglistNew(XvcTree * NewTree, XvcArglist * OldList)
{
	XvcArglist * rVal;
	rVal = malloc(sizeof(XvcArglist));
	XvcCleanupCacheArglist(rVal);
	if (OldList) rVal->depth = OldList->depth + 1;
	else rVal->depth = 1;
	rVal->value = NewTree;
	rVal->next = OldList;
	XvcCleanupReleaseTree(NewTree);
	XvcCleanupReleaseArglist(OldList);
	return rVal;
}

void XvcArglistDelete(XvcArglist * OldArglist)
{
	if (OldArglist) {
		XvcArglistDelete(OldArglist->next);
		XvcTreeDelete(OldArglist->value);
		free(OldArglist);
	 }
}

// Destroy the arglist, but leave the trees contained therein intact.
void XvcArglistDissolve(XvcArglist * OldArglist)
{
	if (OldArglist && OldArglist->next) XvcArglistDissolve(OldArglist->next);
	free(OldArglist);
}

XvcTree ** XvcArglistGetTrees(XvcArglist * InArglist)
{
	XvcTree ** rVal = malloc(InArglist->depth * sizeof(XvcTree *));
	XvcArglist * Current = InArglist;
	int i;
	
	if (rVal) {
		for (i = 0; i < InArglist->depth; i++) {
			rVal[i] = Current->value;
			Current = Current->next;
		}
	}
	return rVal;
}
