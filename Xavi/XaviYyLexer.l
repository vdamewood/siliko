/*
 * XaviYyLexer.l: Flex input.
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

%option bison-bridge reentrant
%option noyywrap nodefault nounput
%{
#include "XaviYyParser.h"

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265
%}
%%
"d" |
"+" |
"-" |
"*" |
"^" |
"/" |
"," |
"(" |
")" { return *yytext; }
[0-9]+ { yylval_param->i = atoi(yytext); return INTEGER; }
[0-9]+\.[0-9]+ { yylval_param->f = atof(yytext); return FLOAT; }
"e"  { yylval_param->f = EULER; return FLOAT; }
"pi" { yylval_param->f = PI;    return FLOAT; }
([a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]*) {
	yylval_param->s = yytext;
	return ID;
}
[ \t\n] {}
. { yylval_param->s = yytext; return ERROR; }
%%
