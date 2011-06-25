%{
#include "tokenizer.h"
#include "parserx.h"
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
calculation: EOL { vldci_set_int(0); }
 | expression EOL { vldci_set_int($1); }
 ;

expression: INTEGER
 | FLOAT
 | expression '+' expression { $$ = $1 + $3; }
 | expression '-' expression { $$ = $1 - $3; }
 | expression '*' expression { $$ = $1 * $3; }
 | expression '/' expression { $$ = $1 / $3; }
 | '-' INTEGER %prec UNEG { $$ = $2 * -1; }
 | expression 'd' INTEGER { $$ = vldci_dice($1, $3); }
 | '(' expression ')' { $$ = $2; }
 | ERROR { YYERROR; }
;

%%
