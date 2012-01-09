/*
 * XvcArglist.h: Functions to manipulate arguments lists for functions.
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

#if !defined XVC_ARGLIST_H
#define XVC_ARGLIST_H

#include "XvcStructs.h"

XvcArglist * XvcArglistNew(XvcTree * NewArgument, XvcArglist * ListToExtend);
void XvcArglistDelete(XvcArglist * ArglistToDelete);
void XvcArglistDissolve(XvcArglist * ArglistToDissolve);
XvcTree ** XvcArglistGetTrees(XvcArglist * ArglistToExtract);

#endif // XVC_ARGLIST_H