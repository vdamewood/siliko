%{
#include "xvcalc.h"
#include "XvcState.h"
#include "tokenize.h"
#include "XvcEvaluate.h"
#include "XvcTree.h"
#include "XvcArglist.h"
#include "XvcFunctionId.h"
	
static void LexError(char);
static void xvcalc_yyerror(const char *);
	
#define xvcalc_new_operation XvcTreeNewOperator
#define xvcalc_new_int       XvcTreeNewInteger
#define xvcalc_new_float     XvcTreeNewFloat
#define xvcalc_new_function  XvcTreeNewFunction
#define xvcalc_make_id       XvcFunctionIdNew
#define xvcalc_add_argument  XvcArglistNew
	
%}

%code requires {
#include "structs.h"
}
%union {
	tree * t;
	arglist * a;
	char * s;
	int i;
	float f;
}

%left '+' '-'
%left '*' '/'
%nonassoc 'd' UNEG

%token <i> INTEGER
%token <f> FLOAT
%token UNEG
%token <s> ID
%token ERROR
%token EOL 0

%type <t> expression fcall
%type <a> arglist
%type <s> id

%%
calculation: EOL { xvcalc_set_nil(); }
 | expression EOL {
	xvcalc_set_value_from_tree($1);
	xvcalc_clear_memory($1);
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
 | fcall
 | ERROR { LexError(*xvcalc_yylval.s); YYERROR; }
;

fcall: id '(' arglist ')' { $$ = xvcalc_new_function($1, $3); };

id: ID { $$ = xvcalc_make_id(xvcalc_yylval.s); }

arglist: expression { $$ = xvcalc_add_argument($1, NULL); }
 | expression ',' arglist { $$ = xvcalc_add_argument($1, $3); };
;
%%

static void LexError(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		xvcalc_set_malloc_error();
		return;
	}
	sprintf(message, t_message, bad_char);
	XvcStateSetErrorMessage(message);
	free(message);
	XvcStateSetStatus(E_SYNTAX);
}

static void xvcalc_yyerror(const char *s)
{	
	XvcStateSetErrorMessage(s);
	XvcStateSetStatus(E_SYNTAX);
}
