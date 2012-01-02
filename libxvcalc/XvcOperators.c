#include <stdlib.h>
#include <time.h>

#include "xvcalc.h" // For error codes
#include "XvcOperators.h" // For xvcalc_call_operator() declarationxcode 

static number add(number left, number right, jmp_buf jb)
{
	number rVal;

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

static number subtract(number left, number right, jmp_buf jb)
{
	number rVal;

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

static number multiply(number left, number right, jmp_buf jb)
{
	number rVal;

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

static number divide(number left, number right, jmp_buf jb)
{
	number rVal;

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

static number dice(number left, number right, jmp_buf jb)
{
	/* TODO: Make this function handle fractional dice. */
	static int has_seeded = 0;
	int running = 0;
	number rVal;
	int count;
	int faces;
	int i;
	
	if (left.type == 'f') count = (int) left.f;
	else count = left.i;

	/* Due to the grammar, right is guarenteed to be an integer. */
	faces = right.i;

	if (!has_seeded) srand(time(NULL));
	for (i = 1; i <= count; i++) running += (rand() % faces) + 1;
	rVal.type = 'i';
	rVal.i = running;
	return rVal;
}

typedef number (*function_ptr)(number, number, jmp_buf);

static function_ptr xvcalc_get_operator(char operator)
{
	switch (operator) {
		case '+': return add;
		case '-': return subtract;
		case '*': return multiply;
		case '/': return divide;
		case 'd': return dice;
	}
	return NULL;
}

/****************************************\
|* Begin module's external interface.   *|
\****************************************/

number xvcalc_call_operator(char operator, number * operands, jmp_buf jb)
{
	function_ptr f;

	f = xvcalc_get_operator(operator);
	if (!f) longjmp(jb, E_FUNCTION);
	return f(operands[0], operands[1], jb);
}
