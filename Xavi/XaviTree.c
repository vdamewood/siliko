/*
 * XaviTree.c: Functions to manipulate abstract syntax trees.
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

#include <stdlib.h>

#include "XaviTree.h"
#include "XaviFunctionCall.h"

int XaviTreeGraftLeft(XaviTree *parent, XaviTree *left)
{
	if (parent->type == 'f')
	{
		if (parent->func->arg_count == 0)
		{
			return 0;
		}
		else if (parent->func->arg_vector[0] == NULL)
		{
			parent->func->arg_vector[0] = left;
			return -1;
		}
		else
		{
			return XaviTreeGraftLeft
			(
				parent->func->arg_vector[0],
				left
			);
		}
	}
	else
	{
		return 0;
	}
}

int XaviTreeNegate(XaviTree *tree)
{
	if (tree == NULL || tree->type != 'n')
		return 0;
	else if (tree->num->status == XS_INTEGER)
		tree->num->i *= -1;
	else if (tree->num->status == XS_FLOAT)
		tree->num->f *= -1.0;
	else
		return 0;
	return 1;
}

XaviTree *XaviTreeNewInteger(int value)
{
	XaviTree *rVal;
	XaviValue *rValNum;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValNum = malloc(sizeof(XaviValue)); // FIXME: if

	if (!rVal || !rValNum)
	{
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	rVal->type = 'n';
	rVal->num->status = XS_INTEGER;
	rVal->num->i = value;
	return rVal;
}

XaviTree *XaviTreeNewFloat(float value)
{
	XaviTree *rVal;
	XaviValue *rValNum;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValNum = malloc(sizeof(XaviValue)); // FIXME: if

	if (!rVal || !rValNum)
	{
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	rVal->type = 'n';
	rVal->num->status = XS_FLOAT;
	rVal->num->f = value;
	return rVal;
}

XaviTree *XaviTreeNewFunction(char *name, int argc, XaviTree **argv)
{
	XaviTree *rVal;
	XaviFunction *rValFunc;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValFunc = malloc(sizeof(XaviFunction)); // FIXME: if

	if (!rVal || !rValFunc)
	{
		free(rVal);
		free(rValFunc);
		return NULL;
	}
	rVal->func = rValFunc;

	rVal->type = 'f';
	rVal->func->name = name;
	rVal->func->arg_count = argc;
	rVal->func->arg_vector = argv;

	return rVal;
}

XaviTree *XaviTreeNewSyntaxError(void)
{
	XaviTree *rVal;
	rVal = malloc(sizeof(XaviTree *));
	rVal->type = 'e';
	return rVal;
}

void XaviTreeDelete(XaviTree *tree)
{
	int i;
	if (tree)
	{
		switch (tree->type)
		{
		case 'n':
			free(tree->num);
			break;
		case 'f':
			free(tree->func->name);
			for (i = 0; i <  tree->func->arg_count; i++)
				XaviTreeDelete(tree->func->arg_vector[i]);
			free(tree->func->arg_vector);
			free(tree->func);
			break;
		}
		free(tree);
	}
}

static int IsNumber(XaviValue n)
{
	return n.status == XS_INTEGER || n.status == XS_FLOAT;
}

static XaviValue EvaluateFunction(XaviFunction *func)
{
	XaviValue rVal;
	XaviValue * arguments = NULL;
	int i;

	if (func->arg_count)
	{
		if (!(arguments =
			malloc(sizeof(XaviValue) * func->arg_count)))
		{
			rVal.status = XE_MEMORY;
			return rVal;
		}

		for(i = 0; i < func->arg_count; i++)
		{
			arguments[i] = XaviTreeEvaluate(func->arg_vector[i]);
			if(!IsNumber(arguments[i]))
			{
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

XaviValue XaviTreeEvaluate(XaviTree *tree)
{
	XaviValue rVal;

	if (!tree || tree->type == 'e')
	{
		rVal.status = XE_SYNTAX;
		return rVal;
	}

	switch (tree->type)
	{
	case 'n':
		return *tree->num;
	case 'f':
		return EvaluateFunction(tree->func);
	}

	rVal.status = XE_INTERNAL;
	return rVal;
}
