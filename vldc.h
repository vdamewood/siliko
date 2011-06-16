#if !defined VLDC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

int vldc(char *inString);
char *vldc_error_message(void);
int vldc_error_status(void);
void vldc_clean(void);

#if defined __cplusplus
}
#endif /* __cplusplus */

#endif /* VLDC_H */
