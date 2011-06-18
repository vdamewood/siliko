#include <stdio.h>
#include "vldc.h"

int main(int argc, char *argv[])
{
	int err_stat = 0;
	char *err_msg = NULL;

	if (argc == 2) {
		printf("Value: %d\n", vldc(argv[1]));
		err_stat = vldc_error_status();
		err_msg = vldc_error_message();
		if(err_stat) {
			printf("Error (%d): ", err_stat);
			if (err_msg) {
				printf("%s\n", err_msg);
			}
			else {
				printf("[No message.]");
			}
		}
		err_msg = NULL;
		vldc_clean();
	}	
	else {
		printf("usage: %s <string to parse>\n", argv[0]);
	}
	return 0;
}
