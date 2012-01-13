/*
 * XvcTestFunctions.c: Program to test XVCalc's function-calling code.
 * Copyright 2012 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <setjmp.h>
#include "XvcFunctionCall.h"

int main()
{
	jmp_buf oops;     // Required for catching exceptions.
	XvcNumber inputs[1]; // Arguments to functions.
	XvcNumber value;     // Return value of function.
	int total = 0;    // Total number of tests run.
	int success = 0;  // Number of successful tests.

	printf("Test 1: abs(-400) returns 400... ");
	inputs[0].type = 'i';
	inputs[0].i = -400;
	total++;
	if(setjmp(oops) == 0) {
		value = XvcFunctionCall("abs", 1, inputs, oops);
		if (value.type == 'i' && value.i == 400) {
			printf("success.\n");
			success++;
		}
		else {
			printf("failed. Value returned was: %c %i %f",
				   value.type, value.i, value.f);
		}
	}
	else {
		printf("failed. Called longjmp().\n");
	}

	////////////////////////////////////////////////////////////////////
	
	// Summary	
	printf("Tests passed/run: %d/%d\n"
		   "Percentage:       %06.2f\n",
		   success, total, ((float) success / (float) total) * 100.00);
	return total - success;
}
