%{
#include "tokenize.h"
#include "xvcalcix.h"
%}

%union {
	struct xvcalc_intx_tree * t;
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
calculation: EOL { xvcalc_intx_set_int(0); }
 | expression EOL {
	xvcalc_intx_set_int(xvcalc_intx_evaluate_tree($1));
	xvcalc_intx_delete_tree($1);
};

expression: INTEGER            { $$ = xvcalc_intx_new_int($1);        }
 | '-'      INTEGER %prec UNEG { $$ = xvcalc_intx_new_int($2 * -1);   }
 |          FLOAT              { $$ = xvcalc_intx_new_float($1);      }
 | '-'      FLOAT %prec UNEG   { $$ = xvcalc_intx_new_int($2 * -1.0); }
 | expression '+' expression { $$ = xvcalc_intx_new_operation('+', $1, $3); }
 | expression '-' expression { $$ = xvcalc_intx_new_operation('-', $1, $3); }
 | expression '*' expression { $$ = xvcalc_intx_new_operation('*', $1, $3); }
 | expression '/' expression { $$ = xvcalc_intx_new_operation('/', $1, $3); }
 | expression 'd' INTEGER {
	$$ = xvcalc_intx_new_operation('d', $1, xvcalc_intx_new_int($3));
   }
 | '(' expression ')' { $$ = $2; }
 | ERROR { YYERROR; }
;

%%
