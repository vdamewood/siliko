#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "xvcalc.h"
#include "functions.h"

number xvcalc_op_add(int argc, number * args, jmp_buf jb)
{
	number rVal;
	
	/* Operators take exactly two arguments. */
	if (argc != 2) longjmp(jb, E_ARGUMENTS);

	if (args[0].type == 'f') {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = args[0].f + args[1].f;
		}
		else {
			rVal.type = 'f';
			rVal.f = args[0].f + (float) args[1].i;
		}
	}
	else {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) args[0].i + args[1].f;
		}
		else {
			rVal.type = 'i';
			rVal.i = args[0].i + args[1].i;
		}
	}
	return rVal;
}

number xvcalc_op_sub(int argc, number * args, jmp_buf jb)
{
	number rVal;
	
	/* Operators take exactly two arguments. */
	if (argc != 2) longjmp(jb, E_ARGUMENTS);

	if (args[0].type == 'f') {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = args[0].f - args[1].f;
		}
		else {
			rVal.type = 'f';
			rVal.f = args[0].f - (float) args[1].i;
		}
	}
	else {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) args[0].i - args[1].f;
		}
		else {
			rVal.type = 'i';
			rVal.i = args[0].i - args[1].i;
		}
	}
	return rVal;
}

number xvcalc_op_mul(int argc, number * args, jmp_buf jb)
{
	number rVal;
	
	/* Operators take exactly two arguments. */
	if (argc != 2) longjmp(jb, E_ARGUMENTS);

	if (args[0].type == 'f') {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = args[0].f * args[1].f;
		}
		else {
			rVal.type = 'f';
			rVal.f = args[0].f * (float) args[1].i;
		}
	}
	else {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) args[0].i * args[1].f;
		}
		else {
			rVal.type = 'i';
			rVal.i = args[0].i * args[1].i;
		}
	}
	return rVal;
}

number xvcalc_op_div(int argc, number * args, jmp_buf jb)
{
	number rVal;
	
	/* Operators take exactly two arguments. */
	if (argc != 2) longjmp(jb, E_ARGUMENTS);

	/* Division-by-Zero Error */
	if ((args[1].type == 'f' && args[1].f == 0.0)
		|| (args[1].type == 'i' && args[1].i == 0)) {
		longjmp(jb, E_ZERO_DIV);
	}

	if (args[0].type == 'f') {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = args[0].f / args[1].f;
		}
		else {
			rVal.type = 'f';
			rVal.f = args[0].f / (float) args[1].i;
		}
	}
	else {
		if (args[1].type == 'f') {
			rVal.type = 'f';
			rVal.f = (float) args[0].i / args[1].f;
		}
		else {
			/* TODO: Make this return a float if it's fractional */
			rVal.type = 'i';
			rVal.i = args[0].i / args[1].i;
		}
	}
	return rVal;
}

number xvcalc_op_dice(int argc, number * args, jmp_buf jb)
{
	/* TODO: Make this function handle fractional dice. */
	static int has_seeded = 0;
	int running = 0;
	number rVal;
	int count;
	int faces;
	int i;
	
	/* Operators take exactly two arguments.*/
	if (argc != 2) longjmp(jb, E_ARGUMENTS);

	if (args[0].type == 'f') count = (int) args[0].f;
	else count = args[0].i;

	/* Due to the grammar, args[1] is guarenteed to be an integer. */
	faces = args[1].i;

	if (!has_seeded) srand(time(NULL));
	for (i = 1; i <= count; i++) running += (rand() % faces) + 1;
	rVal.type = 'i';
	rVal.i = running;
	return rVal;
}

number xvcalc_func_abs(int argc, number * args, jmp_buf jb)
{
	number rVal;

	/* Operators take exactly two arguments. */
	if (argc != 1) longjmp(jb, E_ARGUMENTS);

	rVal = args[0];
	if (rVal.type == 'f')
		rVal.f = fabs(rVal.f);
	else if (rVal.type == 'i')
		rVal.i = abs(rVal.i);
	return rVal;
}

number xvcalc_func_dummy(int argc, number * args, jmp_buf jb)
{
	return args[0];
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
	if (strcmp(name, "abs") == 0)
		return xvcalc_func_abs;
	else
		return xvcalc_func_dummy;
}
