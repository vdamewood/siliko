/*
 * Xavi.h: Public interface for Xavi.
 * Copyright 2012, 2014 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */
#if !defined Xavi_H
#define Xavi_H

#if defined _WIN32 && defined _MSC_VER
	#if defined Xavi_EXPORTS
		#define API __declspec(dllexport)
	#else Xavi_EXPORTS
		#define API __declspec(dllimport)
	#endif // Xavi_EXPORTS
#else // _WIN32 && __MSC_VER
	#define API
#endif

#if defined __cplusplus
extern "C"
{
#endif /* __cplusplus */

enum XaviStatus
{
	NONE = 0,    /* The XaviParse() function hasn't been called. */
	S_INTEGER,   /* Success: The integer result may be retrieved with
	                XaviGetInteger(); */
	S_FLOAT,     /* Success. The floating-point result may be retrieved
	                with XaviGetFloat(); */
	E_INTERNAL,  /* Error: This indicated a bug in Xavi. */
	E_MEMORY,    /* Error: Xavi attempted to allocate memory to
	                process the expression and failed. (i.e.
	                malloc() returned NULL.) */
	E_SYNTAX,    /* Error: A syntax error was encountered. */
	E_ZERO_DIV,  /* Error: Division by zero was attempted. */
	E_FUNCTION,  /* Error: The expression contains a function call that
	                could not be resolved to a valid function. */
	E_ARGUMENTS, /* Error: A function call was made with a number of
	                arguments that the function can't accept. */
	E_DOMAIN,    /* Error: A function call resulted in a domain error. */
	E_RANGE      /* Error: A function call resulted in a range error. */
};
typedef enum XaviStatus XaviStatus;

struct XaviNumber
{
	XaviStatus status;
	union
	{
		int i;
		float f;
	};
};
typedef struct XaviNumber XaviNumber;	
	
API void XaviOpen(void);
API void XaviClose(void);
API XaviNumber XaviParse(const char *StringToParse);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* Xavi_H */
