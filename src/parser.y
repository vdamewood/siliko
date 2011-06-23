%{
#include "parserx.h"

int vldc_yylex(void);
%}

%left '+' '-'
%left '*' '/'
%nonassoc 'd' UNEG

%token NUMBER
%token UNEG
%token EOL 0

%%

calculation: 
 | calculation expression EOL { vldci_set_value($2); }
 ;

expression: NUMBER
 | expression '+' expression { $$ = $1 + $3; }
 | expression '-' expression { $$ = $1 - $3; }
 | expression '*' expression { $$ = $1 * $3; }
 | expression '/' expression { $$ = $1 / $3; }
 | '-' NUMBER %prec UNEG { $$ = $2 * -1 }
 | expression 'd' NUMBER { $$ = vldci_dice($1, $3); }
 | '(' expression ')' { $$ = $2 }
;
%%
