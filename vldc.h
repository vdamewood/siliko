#if !defined VLDC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

int vldc(char *inString);
char *vldc_error(void);
void vldc_clean(void);

#if defined __cplusplus
}
#endif /* __cplusplus */

#endif /* VLDC_H */
