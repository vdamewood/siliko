/*
 * Xavi.h: Public interface for Xavi
 * Copyright 2012, 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
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

enum XaviResultStatus
{
	XAVI_RS_INTEGER = 1,   /* Success: Integer result is in i. */
	XAVI_RS_FLOAT = 2,     /* Success. Floating-point result is in f. */
	XAVI_RE_INTERNAL = -1,  /* Error: This indicates a bug in Xavi. */
	XAVI_RE_MEMORY = -2,    /* Error: Xavi attempted to allocate memory to
	                             process the expression and failed. I.e.
	                             malloc returned NULL. */
	XAVI_RE_SYNTAX = -3,    /* Error: A syntax error was encountered. */
	XAVI_RE_ZERO_DIV = -4,  /* Error: Division by zero was attempted. */
	XAVI_RE_FUNCTION = -5,  /* Error: The expression contains a function
	                             call
	                             that could not be resolved to a valid
	                             function. */
	XAVI_RE_ARGUMENTS = -6, /* Error: A function call was made with a
	                             number of arguments that the function
	                             can't accept. */
	XAVI_RE_DOMAIN = -7,    /* Error: A function call resulted in a domain
	                             error. */
	XAVI_RE_RANGE = -8      /* Error: A function call resulted in a range
	                             error. */
};
typedef enum XaviResultStatus XaviResultStatus;

struct XaviResult
{
	XaviResultStatus status;
	union
	{
		int i;
		float f;
	};
};
typedef struct XaviResult XaviResult;

API void XaviOpen(void);
API void XaviClose(void);
API XaviResult XaviEvaluate(const char *StringToParse);

#if defined __cplusplus
}
#endif /* __cplusplus */
#endif /* Xavi_H */
