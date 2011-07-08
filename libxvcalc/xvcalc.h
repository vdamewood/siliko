#if !defined XVCALC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

char  xvcalc(char *inString);
int   xvcalc_get_int(void);
float xvcalc_get_float(void);
char *xvcalc_error_message(void);
void  xvcalc_clean(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* XVCALC_H */
