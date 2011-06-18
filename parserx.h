#if !defined PARSERX_H
#define PARSERX_H

int   vldci_dice(int,int);
int   vldci_get_value(void);
void  vldci_set_value(int);
int   vldci_get_error_status(void);
void  vldci_set_malloc_error(void);
char *vldci_get_error_message(void);
void  vldci_clean(void);
void  vldc_yyerror(const char *);

#endif /* PARSERX_H */
