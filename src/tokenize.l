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
[ \t] {}
. { xvcalc_report_lex_error(*xvcalc_yytext); return ERROR; }
%%
