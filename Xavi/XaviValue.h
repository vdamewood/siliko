/*
 * XaviValue.c: Data structure to represent a single value
 * Copyright 2014, 2015 Vincent Damewood
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

#if !defined Xavi_VALUE_H
#define Xavi_VALUE_H

enum XaviValueStatus
{
	XS_INTEGER,   /* Success: The value is an integer. */
	XS_FLOAT,     /* Success: The valis is a floating-point */
	XE_INTERNAL,  /* Error: Something unexpected happened. This usually
	                        indicates a bug in Xavi. */
	XE_MEMORY,    /* Error: A call to malloc failed. */
	XE_SYNTAX,    /* Error: A syntax error was encountered. */
	XE_ZERO_DIV,  /* Error: Division by zero was attempted. */
	XE_FUNCTION,  /* Error: A function call could not be resolved to a
	                        valid function. */
	XE_ARGUMENTS, /* Error: A function call was made with a number of
	                        arguments that the function can't accept. */
	XE_DOMAIN,    /* Error: A function call resulted in a domain error. */
	XE_RANGE      /* Error: A function call resulted in a range error. */
};
typedef enum XaviValueStatus XaviValueStatus;

struct XaviValue
{
	XaviValueStatus status;
	union
	{
		int i;
		float f;
	};
};
typedef struct XaviValue XaviValue;

#endif /* Xavi_VALUE_H */
