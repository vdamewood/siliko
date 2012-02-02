/*
 * XaviEvaluate.c: Syntax tree evaluation.
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

#include "Xavi.h"
#include "XaviEvaluate.h"
#include "XaviFunctionCall.h"
#include "XaviOperatorCall.h"

static int IsNumber(XaviNumber n) {
	return n.status == S_INTEGER || n.status == S_FLOAT;
}

static XaviNumber EvaluateOperator(XaviOperator * op)
{
	XaviNumber opLeft;
	XaviNumber opRight;

	opLeft = XaviEvaluate(op->left);
	if (!IsNumber(opLeft)) return opLeft;

	opRight = XaviEvaluate(op->right);
	if (!IsNumber(opRight)) return opRight;

	return XaviOperatorCall(op->symbol, opLeft, opRight);
}

XaviNumber EvaluateFunction(XaviFunction * func)
{
	XaviNumber rVal;
	XaviNumber * arguments = NULL;
	int i;

	if (func->arg_count) {
		arguments = malloc(sizeof(XaviNumber) * func->arg_count);
		
		for(i = 0; i < func->arg_count; i++) {
			arguments[i] = XaviEvaluate(func->arg_vector[i]);
			if(!IsNumber(arguments[i])) {
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}
	
	rVal = XaviFunctionCall(func->name, func->arg_count, arguments);
	free(arguments);
	return rVal;
}

XaviNumber XaviEvaluate(XaviTree * tree)
{
	XaviNumber rVal;

	if (!tree) {
		rVal.status = E_SYNTAX;
		return rVal;
	}

	switch (tree->type) {
	case 'n': return *tree->num;
	case 'o': return EvaluateOperator(tree->op);
	case 'f': return EvaluateFunction(tree->func);
	}
	rVal.status = E_INTERNAL;
	return rVal;
}
