/*
 * XvcYyLexer.l: Flex input.
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

%option noyywrap nodefault nounput
%{
#include "XvcYyParser.h"

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
[0-9]+ { Xvc_yylval.i = atoi(Xvc_yytext); return INTEGER; }
[0-9]+\.[0-9]+ { Xvc_yylval.f = atof(Xvc_yytext); return FLOAT; }
"e"  { Xvc_yylval.f = EULER; return FLOAT; }
"pi" { Xvc_yylval.f = PI;    return FLOAT; }
([a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]*) {
	Xvc_yylval.s = yytext;
	return ID;
}
[ \t\n] {}
. { Xvc_yylval.s = yytext; return ERROR; }
%%
