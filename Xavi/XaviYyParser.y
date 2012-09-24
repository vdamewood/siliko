/*
 * XaviYyParser.y: Bison grammar.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

%define api.pure
%parse-param {XaviNumber * result}
%parse-param {XaviMemoryPool * pool}
%parse-param {yyscan_t scannerState}
%lex-param {yyscan_t scannerState}
%{
#include "Xavi.h"
#include "XaviCleanup.h"
#include "XaviEvaluate.h"
#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviFunctionId.h"
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#include "XaviYyParser.h"
#include "XaviYyLexer.h"

static void Xavi_yyerror(XaviNumber *,
						 XaviMemoryPool *,
						 yyscan_t,
						 const char *);
%}

%code requires {
#include "XaviStructs.h"
}
%union {
	XaviTree * t;
	XaviArglist * a;
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
calculation: EOL { result->status = S_INTEGER; result->i = 0; }
 | expression EOL {
	*result = XaviEvaluate($1);
	XaviCleanupClearAll(pool);
};

expression: INTEGER            { $$ = XaviTreeNewInteger($1, pool); }
 | '-'      INTEGER %prec UNEG { $$ = XaviTreeNewInteger($2 * -1, pool); }
 |          FLOAT              { $$ = XaviTreeNewFloat($1, pool); }
 | '-'      FLOAT %prec UNEG   { $$ = XaviTreeNewFloat($2 * -1.0, pool); }
 | expression '+' expression { $$ = XaviTreeNewOperator(OP_ADD, $1, $3, pool); }
 | expression '-' expression { $$ = XaviTreeNewOperator(OP_SUB, $1, $3, pool); }
 | expression '*' expression { $$ = XaviTreeNewOperator(OP_MUL, $1, $3, pool); }
 | expression '/' expression { $$ = XaviTreeNewOperator(OP_DIV, $1, $3, pool); }
 | expression '^' expression { $$ = XaviTreeNewOperator(OP_POW, $1, $3, pool); }
 | expression 'd' INTEGER {
	$$ = XaviTreeNewOperator(OP_DICE, $1, XaviTreeNewInteger($3, pool), pool);
   }
 | '(' expression ')' { $$ = $2; }
 | fcall
 | ERROR { YYERROR; }
;

fcall: id '(' arglist ')' { $$ = XaviTreeNewFunction($1, $3, pool); };

id: ID { $$ = XaviFunctionIdNew(Xavi_yylval.s, pool); }

arglist: expression { $$ = XaviArglistNew($1, NULL, pool); }
 | expression ',' arglist { $$ = XaviArglistNew($1, $3, pool); };
;
%%

static void Xavi_yyerror(XaviNumber * result,
						 XaviMemoryPool * pool,
						 yyscan_t z,
						 const char *s)
{	
	result->status = E_SYNTAX;
	XaviCleanupClearAll(pool);
}
