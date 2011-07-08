%option noyywrap nodefault
%{
#include "xvcalcix.h"
#include "parser.h"

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
"," |
"(" |
")" { return *yytext; }
[0-9]+ { xvcalc_yylval.i = atoi(xvcalc_yytext); return INTEGER; }
[0-9]+\.[0-9]+ { xvcalc_yylval.f = atof(xvcalc_yytext); return FLOAT; }
"e"  { xvcalc_yylval.f = EULER; return FLOAT; }
"pi" { xvcalc_yylval.f = PI;    return FLOAT; }
([a-ce-z][a-z0-9]*)|(d[a-z][a-z0-9]*) {
	xvcalc_yylval.s = malloc(strlen(xvcalc_yytext)+1);
	strcpy(xvcalc_yylval.s, xvcalc_yytext);
	return ID;
}
[ \t\n] {}
. { xvcalc_report_lex_error(*xvcalc_yytext); return ERROR; }
%%
