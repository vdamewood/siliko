#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "XVCalc.h"
#include "XvcOperatorCall.h"

static XvcNumber Add(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	XvcNumber rVal;

	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f + right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f + (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i + right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i + right.i;
		}
	}
	return rVal;
}

static XvcNumber Subtract(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	XvcNumber rVal;

	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f - right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f - (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i - right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i - right.i;
		}
	}
	return rVal;
}

static XvcNumber Multiply(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	XvcNumber rVal;

	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f * right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f * (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i * right.f;
		}
		else {
			rVal.type = 'i';
			rVal.i = left.i * right.i;
		}
	}
	return rVal;
}

static XvcNumber Divide(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	XvcNumber rVal;

	/* Division-by-Zero Error */
	if ((right.type == 'f' && right.f == 0.0)
		|| (right.type == 'i' && right.i == 0)) {
		longjmp(jb, E_ZERO_DIV);
	}

	if (left.type == 'f') {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = left.f / right.f;
		}
		else {
			rVal.type = 'f';
			rVal.f = left.f / (float) right.i;
		}
	}
	else {
		if (right.type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) left.i / right.f;
		}
		else {
			/* TODO: Make this return a float if it's fractional */
			rVal.type = 'i';
			rVal.i = left.i / right.i;
		}
	}
	return rVal;
}

static XvcNumber Power(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	XvcNumber rVal;
	float myLeft;
	float myRight;

	if (left.type = 'i') {
		myLeft = (float) left.i;
	}
	else {
		myLeft = left.f;
	}

	if (left.type = 'i') {
		myLeft = (float) left.i;
	}
	else {
		myLeft = left.f;
	}

	if (right.type = 'i') {
		myRight = (float) right.i;
	}
	else {
		myRight = right.f;
	}
	
	if (right.type = 'i') {
		myRight = (float) right.i;
	}
	else {
		myRight = right.f;
	}
	
	rVal.type = 'f';
	rVal.f = (float) pow(myLeft, myRight);
	return rVal;
}

static XvcNumber Dice(XvcNumber left, XvcNumber right, jmp_buf jb)
{
	/* TODO: Make this function handle fractional dice. */
	static int has_seeded = 0;
	int running = 0;
	XvcNumber rVal;
	int count;
	int faces;
	int i;
	
	if (left.type == 'f') count = (int) left.f;
	else count = left.i;

	/* Due to the grammar, right is guarenteed to be an integer. */
	faces = right.i;

	if (!has_seeded) srand((unsigned int)time(NULL));
	for (i = 1; i <= count; i++) running += (rand() % faces) + 1;
	rVal.type = 'i';
	rVal.i = running;
	return rVal;
}

typedef XvcNumber (*OperatorPointer)(XvcNumber, XvcNumber, jmp_buf);

static OperatorPointer GetOperator(char operator)
{
	switch (operator) {
		case '+': return Add;
		case '-': return Subtract;
		case '*': return Multiply;
		case '/': return Divide;
		case '^': return Power;
		case 'd': return Dice;
	}
	return NULL;
}

XvcNumber XvcOperatorCall(char operator, XvcNumber left, XvcNumber right, jmp_buf jb)
{
	OperatorPointer f;

	f = GetOperator(operator);
	// FIXME: Give missing operators their own status.
	if (!f) longjmp(jb, E_FUNCTION);
	return f(left, right, jb);
}
