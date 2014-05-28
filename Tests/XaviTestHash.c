#include <stdio.h>
#include <string.h>

unsigned char XaviHash(const unsigned char *rawInput, size_t length)
;
int main(int argc, char * argv[])
{
	if (argc == 2)
	{
		int len = strlen(argv[1]);
		int i = 0;
		for (; i <= len-1; i++)
			printf("%02X", argv[1][i]);
		printf("\n");
		printf("%d\n", (int) XaviHash(argv[1], len));
	}

	return 0;
}
