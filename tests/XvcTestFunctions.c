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
