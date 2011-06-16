%option noyywrap
%{
#include <stdio.h>

#include "parserx.h"

static int error_code = 0;
void report_lex_error(char);

%}

%%
"d" { return DICE; }
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"(" { return OPAREN; }
")" { return CPAREN; }
[0-9]+ { vldclval = atoi(vldctext); return NUMBER; }

[ \t] {}
. { report_lex_error(*vldctext); }
%%
void report_lex_error(char bad_char)
{
	char *message = NULL;
	char *t_message = "Unknown character %c";
	if (!(message = malloc(20))) {
		error_code = 1;
		return;
	}
	sprintf(message, t_message, bad_char);
	vldcerror(message);
	free(message);
}
