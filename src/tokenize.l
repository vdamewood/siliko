%option noyywrap nodefault
%{
#include "parser.h"
#include "xvcalcix.h"
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
[ \t] {}
. { xvcalc_report_lex_error(*xvcalc_yytext); return ERROR; }
%%
