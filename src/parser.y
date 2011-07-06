%{
#include "tokenize.h"
#include "xvcalcix.h"
%}

%union {
	struct xvcalc_tree * t;
	int i;
	float f;
}

%left '+' '-'
%left '*' '/'
%nonassoc 'd' UNEG

%token <i> INTEGER
%token <f> FLOAT
%token UNEG
%token ERROR
%token EOL 0

%type <t> expression

%%
calculation: EOL { xvcalc_set_nil(); }
 | expression EOL {
	xvcalc_set_value(xvcalc_evaluate_tree($1));
	xvcalc_delete_tree($1);
};

expression: INTEGER            { $$ = xvcalc_new_int($1);          }
 | '-'      INTEGER %prec UNEG { $$ = xvcalc_new_int($2 * -1);     }
 |          FLOAT              { $$ = xvcalc_new_float($1);        }
 | '-'      FLOAT %prec UNEG   { $$ = xvcalc_new_float($2 * -1.0); }
 | expression '+' expression { $$ = xvcalc_new_operation('+', $1, $3); }
 | expression '-' expression { $$ = xvcalc_new_operation('-', $1, $3); }
 | expression '*' expression { $$ = xvcalc_new_operation('*', $1, $3); }
 | expression '/' expression { $$ = xvcalc_new_operation('/', $1, $3); }
 | expression 'd' INTEGER {
	$$ = xvcalc_new_operation('d', $1, xvcalc_new_int($3));
   }
 | '(' expression ')' { $$ = $2; }
 | ERROR { YYERROR; }
;

%%
