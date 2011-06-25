#if !defined PARSERX_H
#define PARSERX_H

int   vldci_dice(int,int);
void  vldci_set_int(int);
void  vldci_set_float(float);
void  vldci_set_malloc_error(void);
void  vldci_report_lex_error(char);
void  vldc_yyerror(const char *);

#endif /* PARSERX_H */
