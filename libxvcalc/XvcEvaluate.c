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

XvcNumber XvcEvaluate(XvcTree * tree)
{
	XvcNumber rVal;
	XvcNumber opLeft;
	XvcNumber opRight;
	XvcNumber * EvaluatedArguments = NULL;
	int i;

	if (!tree) {
		rVal.status = E_SYNTAX;
		return rVal;
	}

	switch (tree->type) {
	case 'n':
		rVal = *(tree->num);
		break;
	case 'o':
			opLeft = XvcEvaluate(tree->op->left);
			if (opLeft.status != S_INTEGER &&
				opLeft.status != S_FLOAT) {
				return opLeft;
			}
			opRight = XvcEvaluate(tree->op->right);
			if (opLeft.status != S_INTEGER &&
				opLeft.status != S_FLOAT) {
				return opRight;
			}
			rVal = XvcOperatorCall(tree->op->type,
							   opLeft,
							   opRight);

		break;
	case 'f':
		if (tree->func->arg_count) {
			EvaluatedArguments = malloc(sizeof(XvcNumber)
										* tree->func->arg_count);

			for(i = 0; i < tree->func->arg_count; i++) {
				EvaluatedArguments[i]
					= XvcEvaluate(tree->func->arg_vector[i]);
				if(EvaluatedArguments[i].status != S_INTEGER &&
				   EvaluatedArguments[i].status != S_FLOAT) {
					rVal = EvaluatedArguments[i];
					free(EvaluatedArguments);
					return rVal;
				}
			}
		}

		rVal = XvcFunctionCall(tree->func->name,
									tree->func->arg_count,
									EvaluatedArguments);
		free(EvaluatedArguments);
	}
		
	return rVal;
}
