%option noyywrap
%{
#include <stdio.h>

#include "parser.h"
#include "parserx.h"


void report_lex_error(char);
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
. { report_lex_error(*vldc_yytext); }
%%
void report_lex_error(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		vldci_set_malloc_error();
		return;
	}
	sprintf(message, t_message, bad_char);
	vldc_yyerror(message);
	free(message);
}
