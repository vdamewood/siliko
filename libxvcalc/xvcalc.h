#if !defined XVCALC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

enum xvcalc_status {
	NONE = 0,
	S_INTEGER,
	S_FLOAT,
	E_SYNTAX,
	E_MEMORY,
	E_ZERO_DIV,
	E_FUNCTION,
	E_ARGUMENTS,
	E_DOMAIN,
	E_RANGE
};
typedef enum xvcalc_status xvcalc_status;

xvcalc_status xvcalc(char *inString);
int   xvcalc_get_int(void);
float xvcalc_get_float(void);
char *xvcalc_error_message(void);
void  xvcalc_clean(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* XVCALC_H */
