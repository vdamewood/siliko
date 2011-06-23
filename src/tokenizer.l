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
[0-9]+ { vldc_yylval = atoi(vldc_yytext); return NUMBER; }
[ \t] {}
. { vldci_report_lex_error(*vldc_yytext); }
%%
