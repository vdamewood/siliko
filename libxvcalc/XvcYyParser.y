/*
 * XvcYyParser.y: Bison grammer.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include "XVCalc.h"
#include "XvcState.h"
#include "XvcEvaluate.h"
#include "XvcTree.h"
#include "XvcArglist.h"
#include "XvcFunctionId.h"
#include "XvcYyLexer.h"

static void Xvc_yyerror(const char *);
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
%right '^'
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
calculation: EOL { XvcStateSetNil(); }
 | expression EOL {
	 XvcStateSetValue(XvcEvaluate($1));
	XvcCleanupClearAll($1);
};

expression: INTEGER            { $$ = XvcTreeNewInteger($1); }
 | '-'      INTEGER %prec UNEG { $$ = XvcTreeNewInteger($2 * -1); }
 |          FLOAT              { $$ = XvcTreeNewFloat($1); }
 | '-'      FLOAT %prec UNEG   { $$ = XvcTreeNewFloat($2 * -1.0); }
 | expression '+' expression { $$ = XvcTreeNewOperator('+', $1, $3); }
 | expression '-' expression { $$ = XvcTreeNewOperator('-', $1, $3); }
 | expression '*' expression { $$ = XvcTreeNewOperator('*', $1, $3); }
 | expression '/' expression { $$ = XvcTreeNewOperator('/', $1, $3); }
 | expression '^' expression { $$ = XvcTreeNewOperator('^', $1, $3); }
 | expression 'd' INTEGER {
	$$ = XvcTreeNewOperator('d', $1, XvcTreeNewInteger($3));
   }
 | '(' expression ')' { $$ = $2; }
 | fcall
 | ERROR { YYERROR; }
;

fcall: id '(' arglist ')' { $$ = XvcTreeNewFunction($1, $3); };

id: ID { $$ = XvcFunctionIdNew(Xvc_yylval.s); }

arglist: expression { $$ = XvcArglistNew($1, NULL); }
 | expression ',' arglist { $$ = XvcArglistNew($1, $3); };
;
%%

static void Xvc_yyerror(const char *s)
{	
	XvcStateSetStatus(E_SYNTAX);
	XvcCleanupClearAll();
}
