/*
 * XvcEvaluate.c: Syntax tree evaluation.
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

#include "XVCalc.h"
#include "XvcEvaluate.h"
#include "XvcFunctionCall.h"
#include "XvcOperatorCall.h"

static int IsNumber(XvcNumber n) {
	return n.status == S_INTEGER || n.status == S_FLOAT;
}

static XvcNumber EvaluateOperator(XvcOperator * op)
{
	XvcNumber opLeft;
	XvcNumber opRight;

	opLeft = XvcEvaluate(op->left);
	if (!IsNumber(opLeft)) return opLeft;

	opRight = XvcEvaluate(op->right);
	if (!IsNumber(opRight)) return opRight;

	return XvcOperatorCall(op->type, opLeft, opRight);
}

XvcNumber EvaluateFunction(XvcFunction * func)
{
	XvcNumber rVal;
	XvcNumber * arguments = NULL;
	int i;

	if (func->arg_count) {
		arguments = malloc(sizeof(XvcNumber) * func->arg_count);
		
		for(i = 0; i < func->arg_count; i++) {
			arguments[i] = XvcEvaluate(func->arg_vector[i]);
			if(!IsNumber(arguments[i])) {
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}
	
	rVal = XvcFunctionCall(func->name, func->arg_count, arguments);
	free(arguments);
	return rVal;
}

XvcNumber XvcEvaluate(XvcTree * tree)
{
	XvcNumber rVal;

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
