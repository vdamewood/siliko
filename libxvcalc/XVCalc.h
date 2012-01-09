#if !defined XVCALC_H
#define XVCALC_H

#if defined _WIN32 && defined _MSC_VER
	#if defined xvcalc_EXPORTS
		#define API __declspec(dllexport)
	#else xvcalc_EXPORTS
		#define API __declspec(dllimport)
	#endif // xvcalc_EXPORTS
#else // _WIN32 && __MSC_VER
	#define API
#endif

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
	
API void XvcOpen(void);
API void XvcClose(void);
API XvcStatus XvcParse(char * StringToParse);
API int XvcGetInteger(void);
API float XvcGetFloat(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* XVCALC_H */
