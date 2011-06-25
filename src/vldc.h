#if !defined VLDC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

char vldc(char *inString);
int vldc_get_int(void);
float vldc_get_float(void);
char *vldc_error_message(void);
/*int vldc_error_status(void);*/
void vldc_clean(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* VLDC_H */
