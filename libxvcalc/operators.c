#include <stdlib.h>
#include <time.h>
#include "operatiors.h"

number xvcalc_add(number left, number right)
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

number xvcalc_sub(number left, number right)
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

number xvcalc_mul(number left, number right)
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

number xvcalc_div(number left, number right)
{
	/* TOOD:
	 * Add division by zero check.
	 * Change integer/integer to return a float if needed.
	 */
	number rVal;
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

number xvcalc_dice(number n_count, number n_faces)
{
	number rVal;
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
