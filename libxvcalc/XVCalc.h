/*
 * XVCalc.h: Public interface for XVCalc.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */
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
	E_INTERNAL,  /* Error: This indicated a bug in XVCalc. */
	E_MEMORY,    /* Error: XVCalc attempted to allocate memory to
	                process the expression and failed. (i.e.
	                malloc() returned NULL.) */
	E_SYNTAX,    /* Error: A syntax error was encountered. */
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
API XvcStatus XvcParse(const char * StringToParse);
API int XvcGetInteger(void);
API float XvcGetFloat(void);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* XVCALC_H */
