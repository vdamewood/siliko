/*
 * XaviArglist.h: Functions to manipulate arguments lists for functions.
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

#if !defined Xavi_ARGLIST_H
#define Xavi_ARGLIST_H

#include "XaviStructs.h"

struct XaviArglist {
	int depth;
	XaviTree * value;
	XaviArglist * next;
};
typedef struct XaviArglist XaviArglist;

XaviArglist * XaviArglistNew(XaviTree * NewArgument, XaviArglist * ListToExtend, XaviMemoryPool *);
void XaviArglistDelete(XaviArglist * ArglistToDelete);
void XaviArglistDissolve(XaviArglist * ArglistToDissolve);
XaviTree ** XaviArglistGetTrees(XaviArglist * ArglistToExtract);

#endif // Xavi_ARGLIST_H