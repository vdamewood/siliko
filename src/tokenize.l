%option noyywrap nodefault
%{
#include "parser.h"
#include "xvcalcix.h"

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265
%}
%%
"d" |
"+" |
"-" |
"*" |
"/" |
"(" |
")" { return *yytext; }
[0-9]+ { xvcalc_yylval.i = atoi(xvcalc_yytext); return INTEGER; }
[0-9]+\.[0-9]+ { xvcalc_yylval.f = atof(xvcalc_yytext); return FLOAT; }
"e"  { xvcalc_yylval.f = EULER; return FLOAT; }
"pi" { xvcalc_yylval.f = PI;    return FLOAT; }
[ \t] {}
. { xvcalc_report_lex_error(*xvcalc_yytext); return ERROR; }
%%
