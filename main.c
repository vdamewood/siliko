#include <stdio.h>
#include "vldc.h"

int main(int argc, char *argv[])
{
	if (argc == 2)
		printf("Value: %d\n", vldc(argv[1]));
	else
		printf("usage: %s <string to parse>\n", argv[0]);
	return 0;
}
