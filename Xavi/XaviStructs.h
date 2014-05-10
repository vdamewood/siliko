/*
 * XaviStructs.c: Common data structures.
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

#if !defined Xavi_STRUCTS_H
#define Xavi_STRUCTS_H

#include "Xavi.h"

// Abstract Syntax Tree Structures
struct XaviTree;
struct XaviOperator;
struct XaviFunction;
struct XaviArglist;

typedef struct XaviTree      XaviTree;
typedef struct XaviOperator  XaviOperator;
typedef struct XaviFunction  XaviFunction;
typedef struct XaviArglist   XaviArglist;

// Memory management structures.
struct XaviTreeList;
struct XaviFunctionList;
struct XaviArglistList;

typedef struct XaviTreeList      XaviTreeList;
typedef struct XaviIdList        XaviIdList;
typedef struct XaviArglistList	 XaviArglistList;

#include "XaviArglist.h"
#include "XaviTree.h"

#endif // Xavi_STRUCTS_H
