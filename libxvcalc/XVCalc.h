#if !defined XVCALC_H
#define XVCALC_H
#if defined __cplusplus
extern "C" {
#endif /* __cplusplus */

enum XvcStatus {
	NONE = 0,    /* The XvcParse() function hasn't been called. */
	S_INTEGER,   /* Success: The integer result may be retrieved with
	                XvcGetInteger(); */
	S_FLOAT,     /* Success. The floating-point result may be retrieved
	                with XvcGetFloat(); */
	E_SYNTAX,    /* Error: A syntax error was encountered. */
	E_MEMORY,    /* Error: XVCalc attempted to allocate memory to
	                process the expression and failed. (i.e.
	                malloc() returned NULL.) */
	E_ZERO_DIV,  /* Error: Division by zero was attempted. */
	E_FUNCTION,	 /* Error: The expression contains a function call that
	                could not be resolved to a valid function. */
	E_ARGUMENTS, /* Error: A function call was made with a number of
	                arguments that the function can't accept. */
	E_DOMAIN,    /* Error: A function call resulted in a domain error. */
	E_RANGE      /* Error: A function call resulted in a range error. */
};
typedef enum XvcStatus XvcStatus;
	
void XvcOpen(void);
void XvcClose(void);
XvcStatus XvcParse(char * StringToParse);
int XvcGetInteger(void);
float XvcGetFloat(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* XVCALC_H */
