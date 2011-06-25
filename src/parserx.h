#if !defined PARSERX_H
#define PARSERX_H

int   xvcalc_intx_dice(int,int);
void  xvcalc_intx_set_int(int);
void  xvcalc_intx_set_float(float);
void  xvcalc_intx_set_malloc_error(void);
void  vxcalc_intx_report_lex_error(char);
void  xvcalc_intx_yyerror(const char *);

#endif /* PARSERX_H */
