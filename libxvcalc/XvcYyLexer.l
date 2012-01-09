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
