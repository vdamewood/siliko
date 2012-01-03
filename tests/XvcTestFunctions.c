#include <stdio.h>
#include <setjmp.h>
#include "XvcFunctions.h"

int main()
{
	jmp_buf oops;     // Required for catching exceptions.
	number inputs[3]; // Arguments to functions.
	number value;     // Return value of function.
	int total = 0;    // Total number of tests run.
	int success = 0;  // Number of successful tests.

	////////////////////////////////////////////////////////////////////
	
	printf("Test 0.1: dummy(329) returns 329... ");
	inputs[0].type = 'i';
	inputs[0].i = 329;
	total++;
	if(setjmp(oops) == 0) {
		value = xvcalc_call_function("dummy", 1, inputs, oops);
		if (value.type == 'i' && value.i == 329) {
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
	
	printf("Test 0.2: dummy(401.625, 313, -5) returns 401.25... ");
	inputs[0].type = 'f';
	inputs[0].f = 401.625;
	inputs[1].type = 'i';
	inputs[1].i = 313;
	inputs[2].type = 'i';
	inputs[2].i = -5;
	total++;
	if(setjmp(oops) == 0) {
		value = xvcalc_call_function("dummy", 3, inputs, oops);
		if (value.type == 'f' && value.f == 401.625) {
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

	printf("Test 1: abs(-400) returns 400... ");
	inputs[0].type = 'i';
	inputs[0].i = -400;
	total++;
	if(setjmp(oops) == 0) {
		value = xvcalc_call_function("abs", 1, inputs, oops);
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
