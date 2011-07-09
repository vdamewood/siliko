#if !defined XVCALC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

enum xvcalc_status {
	NONE = 0,	/* The xvcalc() function hasn't been called. */
	S_INTEGER,	/* Success: The integer result may be retrieved with
				xvcalc_get_integer(); */
	S_FLOAT,	/* Success. The floating-point result may be retrieved
				with xvcalc_get_float(); */
	E_SYNTAX,	/* Error: A syntax error was encountered. */
	E_MEMORY,	/* Error: XVCalc attempted to allocate memory to
				process the expression and failed. (i.e.
				malloc() returned NULL. */
	E_ZERO_DIV,	/* Error: Division by zero was attempted. */
	E_FUNCTION,	/* Error: The expression contains a function call that
				could not be resolved to a valid function. */
	E_ARGUMENTS,	/* Error: A function call was made with a number of
				arguments that the function can't accept. */
	E_DOMAIN,	/* Error: Not yet used. */
	E_RANGE		/* Error: Not yet used. */
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
