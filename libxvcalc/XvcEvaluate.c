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

#include <setjmp.h>
#include <stdlib.h>

#include "XVCalc.h"
#include "XvcEvaluate.h"
#include "XvcFunctionCall.h"
#include "XvcState.h"
#include "XvcOperatorCall.h"

static XvcNumber Evaluate(XvcTree * tree, jmp_buf escape);

void XvcEvaluate(XvcTree * in)
{
	jmp_buf escape;
	XvcStatus jmp_value = 0;
	if ((jmp_value = setjmp(escape)) == 0) {
		XvcStateSetValue(Evaluate(in, escape));
	}
	else {
		XvcStateSetNil();
		XvcStateSetStatus(jmp_value);
	}
}

static XvcNumber Evaluate(XvcTree * tree, jmp_buf escape)
{
	XvcNumber rVal;
	XvcNumber * EvaluatedArguments = NULL;
	int i;
	if (!tree) longjmp(escape, E_SYNTAX);

	switch (tree->type) {
	case 'n':
		rVal = *(tree->num);
		break;
	case 'o':
		rVal = XvcOperatorCall(tree->op->type,
							   Evaluate(tree->op->left, escape),
							   Evaluate(tree->op->right, escape),
							   escape);

		break;
	case 'f':
		if (tree->func->arg_count) {
			EvaluatedArguments = malloc(sizeof(XvcNumber)
										* tree->func->arg_count);

			for(i = 0; i < tree->func->arg_count; i++) {
				EvaluatedArguments[i]
					= Evaluate(tree->func->arg_vector[i], escape);
			}
		}

		rVal = XvcFunctionCall(tree->func->name,
									tree->func->arg_count,
									EvaluatedArguments,
									escape);
		free(EvaluatedArguments);
	}
		
	return rVal;
}
