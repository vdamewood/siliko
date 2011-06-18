#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "parserx.h"

static int value;
static char *error_message = NULL;
static int error_status = 0;

int vldci_dice(int count, int faces)
{
	static int hasSeeded = 0;
	int i;
	int running = 0;
	if (!hasSeeded)
		srand(time(NULL));

	for (i = 1; i <= count; i++)
		running += (rand() % faces) + 1;

	return running;
}

int vldci_get_value()
{
	return value;
}

void vldci_set_value(int new_value)
{
	value = new_value;
}

char *vldci_get_error_message(void)
{
        return error_message;
}

int vldci_get_error_status(void)
{
        return error_status;
}

void vldci_set_malloc_error(void)
{
	error_status = 1;
}
void vldci_clean(void)
{
        if(error_message) {
		free(error_message);
		error_message = NULL;
	};
}

void vldc_yyerror(const char *s)
{
	if(!(error_message = malloc(strlen(s)+1))) {
		vldci_set_malloc_error();
		return;
	}
	
        strcpy(error_message, s);
	error_status = 2;
}
