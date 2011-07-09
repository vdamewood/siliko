#include <string.h>
#include "xvcalc.h"
#include "functions.h"

number xvcalc_op_add(int count, number * arguments, jmp_buf jb)
{
	number rVal;
	number left = arguments[0];
	number right = arguments[1];
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

number xvcalc_op_sub(int count, number * arguments, jmp_buf jb)
{
	number rVal;
	number left = arguments[0];
	number right = arguments[1];
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

number xvcalc_op_mul(int count, number * arguments, jmp_buf jb)
{
	number rVal;
	number left = arguments[0];
	number right = arguments[1];
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

number xvcalc_op_div(int count, number * arguments, jmp_buf jb)
{
	/* TOOD:
	 * Add division by zero check.
	 * Change integer/integer to return a float if needed.
	 */
	number rVal;
	number left = arguments[0];
	number right = arguments[1];
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
			rVal.type = 'i';
			rVal.i = left.i / right.i;
		}
	}
	return rVal;
}

number xvcalc_op_dice(int countx, number * arguments, jmp_buf jb)
{
	number rVal;
	number n_count = arguments[0];
	number n_faces = arguments[1];
	int count;
	int faces;
	if (n_count.type == 'f') count = (int) n_count.f;
	else count = n_count.i;
	faces = n_faces.i;

	static int hasSeeded = 0;
	int i;
	int running = 0;
	if (!hasSeeded)
		srand(time(NULL));

	for (i = 1; i <= count; i++)
		running += (rand() % faces) + 1;
	rVal.type = 'i';
	rVal.i = running;
	return rVal;
}

number xvcalc_func_abs(int count, number * arguments, jmp_buf jb)
{
	number rVal;
	rVal = arguments[0];
	if (rVal.type == 'f' && rVal.f < 0.0)
		rVal.f = rVal.f * -1.0;
	else if (rVal.type == 'i' && rVal.i < 0)
		rVal.i = rVal.i * -1;
	return rVal;
}

function_ptr xvcalc_get_operator(char operator)
{
	function_ptr rVal;
	switch (operator) {
	case '+':
		rVal = xvcalc_op_add;
		break;
	case '-':
		rVal = xvcalc_op_sub;
		break;
	case '*':
		rVal = xvcalc_op_mul;
		break;
	case '/':
		rVal = xvcalc_op_div;
		break;
	case 'd':
		rVal = xvcalc_op_dice;
		break;
	}
	return rVal;
}

function_ptr xvcalc_get_function(char * name)
{
	return xvcalc_func_abs;
}