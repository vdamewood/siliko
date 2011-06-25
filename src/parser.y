%{
#include "tokenize.h"
#include "xvcalcix.h"
%}
%left '+' '-'
%left '*' '/'
%nonassoc 'd' UNEG

%token INTEGER
%token FLOAT
%token UNEG
%token ERROR
%token EOL 0
%%
calculation: EOL { xvcalc_intx_set_int(0); }
 | expression EOL { xvcalc_intx_set_int($1); }
 ;

expression: INTEGER
 | FLOAT
 | expression '+' expression { $$ = $1 + $3; }
 | expression '-' expression { $$ = $1 - $3; }
 | expression '*' expression { $$ = $1 * $3; }
 | expression '/' expression { $$ = $1 / $3; }
 | '-' INTEGER %prec UNEG { $$ = $2 * -1; }
 | '-' FLOAT %prec UNEG   { $$ = $2 * -1.0; }
 | expression 'd' INTEGER { $$ = xvcalc_intx_dice($1, $3); }
 | '(' expression ')' { $$ = $2; }
 | ERROR { YYERROR; }
;

%%
