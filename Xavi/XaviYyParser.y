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

%type <t> unumber number fcall atom expr_0 expr_0r expr_1 expr_1r expr_2 expr_2lf expr_3 expr_3lf
%type <a> arglist arglist_lf
%type <s> id

%%
calculation: EOL { result->status = S_INTEGER; result->i = 0; }
 | expr_0 EOL {
	*result = XaviEvaluate($1);
	XaviCleanupClearAll(pool);
};

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

expr_2: expr_3 expr_2lf
{
	if ($2 == NULL)
		$$ = $1;
	else
		$$ = XaviTreeNewOperator(OP_POW, $1, $2, pool);
}

expr_2lf: { $$ = NULL; }
 |	'^' expr_2 { $$ = $2; }

expr_3: atom expr_3lf
	{
		$$ = ($2 == NULL)
			? $1
			: XaviTreeNewOperator(OP_DICE, $1, $2, pool);
	}

expr_3lf:             { $$ = NULL; }
|         'd' INTEGER { $$ = XaviTreeNewInteger($2, pool); }

atom: number
 |	'(' expr_0 ')' { $$ = $2; }
 |	fcall
 |	ERROR { YYERROR; }

number:	unumber
 |	'-' unumber
{
	if (!XaviTreeNegate($2))
	{
		YYERROR;
	}
	$$ = $2;
}

unumber: INTEGER     { $$ = XaviTreeNewInteger($1, pool); }
|        FLOAT       { $$ = XaviTreeNewFloat($1, pool); }

fcall: id '(' arglist ')' { $$ = XaviTreeNewFunction($1, $3, pool); };

id: ID { $$ = XaviFunctionIdNew(Xavi_yylval.s, pool); }

arglist: expr_0 arglist_lf { $$ = XaviArglistNew($1, $2, pool); }

arglist_lf: { $$ = NULL; }
 | ',' arglist { $$ = $2; }
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
