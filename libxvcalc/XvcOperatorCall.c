/*
 * XvcOperatorCall.c: Operator handling.
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
#include <time.h>
#include <math.h>

#include "XVCalc.h"
#include "XvcOperatorCall.h"

static XvcNumber Add(XvcNumber left, XvcNumber right)
{
	XvcNumber rVal;

	if (left.status == S_FLOAT) {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = left.f + right.f;
		}
		else {
			rVal.status = S_FLOAT;
			rVal.f = left.f + (float) right.i;
		}
	}
	else {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = (float) left.i + right.f;
		}
		else {
			rVal.status = S_INTEGER;
			rVal.i = left.i + right.i;
		}
	}
	return rVal;
}

static XvcNumber Subtract(XvcNumber left, XvcNumber right)
{
	XvcNumber rVal;

	if (left.status == S_FLOAT) {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = left.f - right.f;
		}
		else {
			rVal.status = S_FLOAT;
			rVal.f = left.f - (float) right.i;
		}
	}
	else {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = (float) left.i - right.f;
		}
		else {
			rVal.status = S_INTEGER;
			rVal.i = left.i - right.i;
		}
	}
	return rVal;
}

static XvcNumber Multiply(XvcNumber left, XvcNumber right)
{
	XvcNumber rVal;

	if (left.status == S_FLOAT) {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = left.f * right.f;
		}
		else {
			rVal.status = S_FLOAT;
			rVal.f = left.f * (float) right.i;
		}
	}
	else {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = (float) left.i * right.f;
		}
		else {
			rVal.status = S_INTEGER;
			rVal.i = left.i * right.i;
		}
	}
	return rVal;
}

static XvcNumber Divide(XvcNumber left, XvcNumber right)
{
	XvcNumber rVal;

	/* Division-by-Zero Error */
	if ((right.status == S_FLOAT && right.f == 0.0)
		|| (right.status == S_INTEGER && right.i == 0)) {
		rVal.status = E_ZERO_DIV;
		return rVal;
	}

	if (left.status == S_FLOAT) {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = left.f / right.f;
		}
		else {
			rVal.status = S_FLOAT;
			rVal.f = left.f / (float) right.i;
		}
	}
	else {
		if (right.status == S_FLOAT) {
			rVal.status = S_FLOAT;
			rVal.f = (float) left.i / right.f;
		}
		else {
			/* TODO: Make this return a float if it's fractional */
			rVal.status = S_INTEGER;
			rVal.i = left.i / right.i;
		}
	}
	return rVal;
}

static XvcNumber Power(XvcNumber left, XvcNumber right)
{
	XvcNumber rVal;
	float myLeft;
	float myRight;

	if (left.status = S_INTEGER) {
		myLeft = (float) left.i;
	}
	else {
		myLeft = left.f;
	}

	if (left.status = S_INTEGER) {
		myLeft = (float) left.i;
	}
	else {
		myLeft = left.f;
	}

	if (right.status = S_INTEGER) {
		myRight = (float) right.i;
	}
	else {
		myRight = right.f;
	}
	
	if (right.status = S_INTEGER) {
		myRight = (float) right.i;
	}
	else {
		myRight = right.f;
	}
	
	rVal.status = S_FLOAT;
	rVal.f = (float) pow(myLeft, myRight);
	return rVal;
}

static XvcNumber Dice(XvcNumber left, XvcNumber right)
{
	/* TODO: Make this function handle fractional dice. */
	static int has_seeded = 0;
	int running = 0;
	XvcNumber rVal;
	int count;
	int faces;
	int i;
	
	if (left.status == S_FLOAT) count = (int) left.f;
	else count = left.i;

	/* Due to the grammar, right is guarenteed to be an integer. */
	faces = right.i;

	if (!has_seeded) {
		has_seeded = 1;
		srand((unsigned int)time(NULL));
	}

	for (i = 1; i <= count; i++) running += (rand() % faces) + 1;
	rVal.status = S_INTEGER;
	rVal.i = running;
	return rVal;
}

typedef XvcNumber (*OperatorPointer)(XvcNumber, XvcNumber);
static OperatorPointer * Operators = NULL;

int XvcOperatorCallOpen()
{
	if (!(Operators = malloc(OP_TOTAL_COUNT * sizeof(OperatorPointer)))) {
		return 0;
	}
	Operators[OP_ADD] = Add;
	Operators[OP_SUB] = Subtract;
	Operators[OP_MUL] = Multiply;
	Operators[OP_DIV] = Divide;
	Operators[OP_POW] = Power;
	Operators[OP_DICE] = Dice;
	return 1;
}

void XvcOperatorCallClose()
{
	free(Operators);
}

XvcNumber XvcOperatorCall(XvcOperatorSymbol op, XvcNumber left, XvcNumber right)
{
	OperatorPointer f;
	XvcNumber rVal;

	if (op < OP_TOTAL_COUNT) {
		f = Operators[op];
		rVal = f(left, right);
	}
	else {
		rVal.status = E_INTERNAL;
	}
	return rVal;
}
