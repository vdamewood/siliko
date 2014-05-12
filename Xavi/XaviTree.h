/*
 * XaviTree.h: Functions to manipulate abstract syntax trees.
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

#if !defined Xavi_TREE_H
#define Xavi_TREE_H

#include "Xavi.h"

/*enum XaviOperatorSymbol {
	OP_ADD=0,
	OP_SUB=1,
	OP_MUL=2,
	OP_DIV=3,
	OP_POW=4,
	OP_DICE=5,
	OP_TOTAL_COUNT
};
typedef enum XaviOperatorSymbol XaviOperatorSymbol;
*/
struct XaviTree;
typedef struct XaviTree XaviTree;

/*struct XaviOperator {
	XaviOperatorSymbol symbol;
	XaviTree * left;
	XaviTree * right;
	XaviNumber * args;
};
typedef struct XaviOperator XaviOperator;*/

struct XaviFunction {
	char * name;
	int arg_count;
	XaviTree ** arg_vector;
};
typedef struct XaviFunction XaviFunction;


struct XaviTree {
	char type;
	union {
		XaviNumber * num;
		//XaviOperator * op;
		XaviFunction * func;
	};
};
typedef struct XaviTree XaviTree;



XaviNumber XaviTreeEvaluate(XaviTree * TreeToEvaluate);
int XaviTreeGraftLeft(XaviTree * parent, XaviTree * left);
int XaviTreeNegate(XaviTree *);
XaviTree * XaviTreeNewInteger(int Value);
XaviTree * XaviTreeNewFloat(float Value);
XaviTree * XaviTreeNewFunction(char * FunctionName, int argumentCount, XaviTree ** arguments);
XaviTree * XaviTreeNewSyntaxError(void);
void XaviTreeDelete(XaviTree * TreeToDelete);

#endif // Xavi_TREE_H
