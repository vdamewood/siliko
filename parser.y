%{
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "parserx.h"

int vldc_yylex(void);
%}

%token NUMBER
%token ADD SUB MUL DIV DICE
%token OPAREN CPAREN
%token FUNCTION

%%

calculation: 
 | calculation expression { vldci_set_value($2); }
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
 | expression2 DICE value { $$ = vldci_dice($1, $3); }
 ;

value: NUMBER
 | SUB NUMBER { $$ = $2 * -1 }
 | OPAREN expression CPAREN { $$ = $2 }

/* numeric: NUMBER
 | negnumber

negnumber: SUB NUMBER { $$ = $2 * -1 } */
%%
