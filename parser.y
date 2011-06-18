%{
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "parserx.h"
static int value;

int vldc_yylex(void);
int dice(int,int);
%}

%token NUMBER
%token ADD SUB MUL DIV DICE
%token OPAREN CPAREN
%token FUNCTION

%%

calculation: 
 | calculation expression { value = $2; }
 ;

expression: expression1
 | expression ADD expression1 { $$ = $1 + $3; }
 | expression SUB expression1 { $$ = $1 - $3; }
 ;

expression1: expression2
 | expression1 MUL expression2 { $$ = $1 * $3; }
 | expression1 DIV expression2 { $$ = $1 / $3; }
 ;

expression2: value
 | expression2 DICE value { $$ = dice($1, $3); }
 ;

value: NUMBER
 | SUB NUMBER { $$ = $2 * -1 }
 | OPAREN expression CPAREN { $$ = $2 }

/* numeric: NUMBER
 | negnumber

negnumber: SUB NUMBER { $$ = $2 * -1 } */
%%

/*void vldcerror(const char *s)
{
	char **error = vldc_get_error_location();
	if (!(*error = malloc(strlen(s)+1))) {
		vldc_set_error_code(1);
	}
	fprintf(stderr, "error: %s\n", s);
}
*/
int dice(int count, int faces)
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

int vldc_getvalue()
{
	return value;
}
