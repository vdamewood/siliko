%option noyywrap
%{
#include "parserx.h"
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
. { vldcerror("Unknown character in equation."); }
%%
