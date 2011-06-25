%option noyywrap
%{
#include "parser.h"
#include "parserx.h"
%}
%%
"d" |
"+" |
"-" |
"*" |
"/" |
"(" |
")" { return *yytext; }
[0-9]+ { xvcalc_intx_yylval = atoi(xvcalc_intx_yytext); return INTEGER; }
[ \t] {}
. { xvcalc_intx_report_lex_error(*xvcalc_intx_yytext); return ERROR; }
%%
