/*
 * XvcStructs.c: Common data structures.
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

#if !defined XVC_STRUCTS_H
#define XVC_STRUCTS_H

#include "XVCalc.h"

struct XvcTree;
struct XvcOperator;
struct XvcFunction;
struct XvcArglist;

typedef struct XvcTree      XvcTree;
typedef struct XvcOperator  XvcOperator;
typedef struct XvcFunction  XvcFunction;
typedef struct XvcArglist	XvcArglist;

enum XvcOperatorSymbol {
	OP_ADD=0,
	OP_SUB=1,
	OP_MUL=2,
	OP_DIV=3,
	OP_POW=4,
	OP_DICE=5,
	OP_TOTAL_COUNT
};
typedef enum XvcOperatorSymbol XvcOperatorSymbol;

struct XvcTree {
	char type;
	union {
		XvcNumber * num;
		XvcOperator * op;
		XvcFunction * func;
	};
};

struct XvcOperator {
	XvcOperatorSymbol symbol;
	XvcTree * left;
	XvcTree * right;
	XvcNumber * args;
};

struct XvcFunction {
	char * name;
	int arg_count;
	XvcTree ** arg_vector;
};

struct XvcArglist {
	int depth;
	XvcTree * value;
	XvcArglist * next;
};

#endif // XVC_STRUCTS_H
