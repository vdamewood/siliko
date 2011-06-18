#if !defined PARSERX_H
#define PARSERX_H

#include "parser.h"
void vldc_yyerror(const char *);
int  vldc_yyparse(void); 
int  vldc_getvalue(void);

#endif /* PARSERX_H */
