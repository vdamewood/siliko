%{
#include "XVCalc.h"
#include "XvcState.h"
#include "XvcEvaluate.h"
#include "XvcTree.h"
#include "XvcArglist.h"
#include "XvcFunctionId.h"
#include "XvcYyLexer.h"

static void Xvc_yyerror(const char *);
	
#define xvcalc_new_operation XvcTreeNewOperator
#define xvcalc_new_int       XvcTreeNewInteger
#define xvcalc_new_float     XvcTreeNewFloat
#define xvcalc_new_function  XvcTreeNewFunction
#define xvcalc_make_id       XvcFunctionIdNew
#define xvcalc_add_argument  XvcArglistNew
#define xvcalc_set_nil       XvcStateSetNil
#define xvcalc_set_value_from_tree XvcEvaluate
	
#define XvcMemoryCleanup				XvcCleanupClearAll
#define xvcalc_cache_dangling_arglist	XvcCleanupCacheArglist
#define xvcalc_release_dangling_arglist	XvcCleanupReleaseArglist
#define xvcalc_clear_dangling_arglists	XvcCleanupClearArglists
#define xvcalc_cache_dangling_tree		XvcCleanupCacheTree
#define xvcalc_release_dangling_tree	XvcCleanupReleaseTree
#define xvcalc_clear_dangling_trees		XvcCleanupClearTrees
#define xvcalc_cache_dangling_id		XvcCleanupCacheFunctionId
#define xvcalc_release_dangling_id		XvcCleanupReleaseFunctionId
#define xvcalc_clear_dangling_id		XvcCleanupClearFunctionIds
	
%}

%code requires {
#include "XvcStructs.h"
typedef struct XvcTree      tree;
typedef struct XvcOperator  operation;
typedef struct XvcNumber    number;
typedef struct XvcFunction  function;
typedef struct XvcArglist	arglist;
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
	XvcMemoryCleanup($1);
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
 | ERROR { YYERROR; }
;

fcall: id '(' arglist ')' { $$ = xvcalc_new_function($1, $3); };

id: ID { $$ = xvcalc_make_id(Xvc_yylval.s); }

arglist: expression { $$ = xvcalc_add_argument($1, NULL); }
 | expression ',' arglist { $$ = xvcalc_add_argument($1, $3); };
;
%%

static void Xvc_yyerror(const char *s)
{	
	XvcStateSetStatus(E_SYNTAX);
	XvcMemoryCleanup();
}
