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
#include <stdlib.h>

#include "Xavi.h"
#include "XaviCleanup.h"
#include "XaviEvaluate.h"
#include "XaviTree.h"
#include "XaviArglist.h"
#include "XaviFunctionId.h"
#include "XaviLexer.h"

static void Xavi_yyerror(
	XaviNumber *,
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

%token <i> INTEGER
%token <f> FLOAT
%token <s> ID
%token ERROR
%token EOL 0

%type <t> number fcall atom expr_0 expr_0r expr_1 expr_1r expr_2 expr_3
%type <a> arglist
%type <s> id

%%
calculation: EOL { result->status = S_INTEGER; result->i = 0; }
 | expr_0 EOL {
	*result = XaviEvaluate($1);
	XaviCleanupClearAll(pool);
};

/*expr:	expr_1
 |	expr '+' expr_1 { $$ = XaviTreeNewOperator(OP_ADD, $1, $3, pool); }
 |	expr '-' expr_1 { $$ = XaviTreeNewOperator(OP_SUB, $1, $3, pool); }*/

expr_0: expr_1 expr_0r
{
	if ($2 != NULL)
	{
		XaviTreeGraftLeft($2, $1, pool);
		$$ = $2;
	}
	else
	{
		$$ = $1;
	}
}
 
expr_0r: { $$ = NULL; }
 | '+' expr_1 expr_0r
{
	if ($3 != NULL)
	{
		
		XaviTreeGraftLeft(
			$3,
			XaviTreeNewOperator(OP_ADD, NULL, $2, pool),
			pool);
		$$ = $3;
	}
	else
	{
		$$ = XaviTreeNewOperator(OP_ADD, NULL, $2, pool);
	}
}
 | '-' expr_1 expr_0r
{
	if ($3 != NULL)
	{
		
		XaviTreeGraftLeft(
			$3,
			XaviTreeNewOperator(OP_SUB, NULL, $2, pool),
			pool);
		$$ = $3;
	}
	else
	{
		$$ = XaviTreeNewOperator(OP_SUB, NULL, $2, pool);
	}
}

expr_1: expr_2 expr_1r
{
	if ($2 != NULL)
	{
		XaviTreeGraftLeft($2, $1, pool);
		$$ = $2;
	}
	else
	{
		$$ = $1;
	}
}

expr_1r: { $$ = NULL; }
 | '*' expr_2 expr_1r
{
	if ($3 != NULL)
	{
		
		XaviTreeGraftLeft(
			$3,
			XaviTreeNewOperator(OP_MUL, NULL, $2, pool),
			pool);
		$$ = $3;
	}
	else
	{
		$$ = XaviTreeNewOperator(OP_MUL, NULL, $2, pool);
	}
}
 | '/' expr_2 expr_1r
{
	if ($3 != NULL)
	{
		
		XaviTreeGraftLeft(
			$3,
			XaviTreeNewOperator(OP_DIV, NULL, $2, pool),
			pool);
		$$ = $3;
	}
	else
	{
		$$ = XaviTreeNewOperator(OP_DIV, NULL, $2, pool);
	}
}

expr_2: expr_3
 |	expr_3 '^' expr_2 { $$ = XaviTreeNewOperator(OP_POW, $1, $3, pool); }

expr_3: atom
 |	atom 'd' INTEGER
	{
		$$ =
		XaviTreeNewOperator(
			OP_DICE,
			$1,
			XaviTreeNewInteger($3, pool),
			pool);
	}

atom: number
 |	'(' expr_0 ')' { $$ = $2; }
 |	fcall
 |	ERROR { YYERROR; }

number: INTEGER     { $$ = XaviTreeNewInteger($1, pool); }
 |	'-' INTEGER { $$ = XaviTreeNewInteger($2 * -1, pool); }
 |      FLOAT       { $$ = XaviTreeNewFloat($1, pool); }
 |	'-' FLOAT   { $$ = XaviTreeNewFloat($2 * -1.0, pool); }


fcall: id '(' arglist ')' { $$ = XaviTreeNewFunction($1, $3, pool); };

id: ID { $$ = XaviFunctionIdNew(Xavi_yylval.s, pool); }

arglist: expr_0 { $$ = XaviArglistNew($1, NULL, pool); }
 | expr_0 ',' arglist { $$ = XaviArglistNew($1, $3, pool); };
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
