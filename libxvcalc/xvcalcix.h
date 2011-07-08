#if !defined PARSERX_H
#define PARSERX_H
#include "structs.h"

void  xvcalc_set_nil();
void  xvcalc_set_value(number);
void  xvcalc_set_malloc_error(void);
void  vxcalc_report_lex_error(char);
void  xvcalc_yyerror(const char *);

#endif /* PARSERX_H */
