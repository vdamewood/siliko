/*
 * XaviStructs.c: Common data structures.
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

#if !defined Xavi_STRUCTS_H
#define Xavi_STRUCTS_H

#include "Xavi.h"

struct XaviTree;
struct XaviOperator;
struct XaviFunction;
struct XaviArglist;

typedef struct XaviTree      XaviTree;
typedef struct XaviOperator  XaviOperator;
typedef struct XaviFunction  XaviFunction;
typedef struct XaviArglist	XaviArglist;

enum XaviOperatorSymbol {
	OP_ADD=0,
	OP_SUB=1,
	OP_MUL=2,
	OP_DIV=3,
	OP_POW=4,
	OP_DICE=5,
	OP_TOTAL_COUNT
};
typedef enum XaviOperatorSymbol XaviOperatorSymbol;

struct XaviTree {
	char type;
	union {
		XaviNumber * num;
		XaviOperator * op;
		XaviFunction * func;
	};
};

struct XaviOperator {
	XaviOperatorSymbol symbol;
	XaviTree * left;
	XaviTree * right;
	XaviNumber * args;
};

struct XaviFunction {
	char * name;
	int arg_count;
	XaviTree ** arg_vector;
};

struct XaviArglist {
	int depth;
	XaviTree * value;
	XaviArglist * next;
};

#endif // Xavi_STRUCTS_H
