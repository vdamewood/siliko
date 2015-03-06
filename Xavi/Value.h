/*
 * Value.c: Data structure to represent a single value
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

#if !defined XAVI_VALUE_H
#define XAVI_VALUE_H

#include "W32Dll.h"

#if defined __cplusplus
extern "C" {
#endif

enum XAVI_API XaviValueStatus
{
	XAVI_VAL_INTEGER,       /* Success: The value is an integer. */
	XAVI_VAL_FLOAT,         /* Success: The valis is a float. */
	XAVI_VAL_MEMORY_ERR,    /* Error: An attempt to allocate memory
				    failed. */
	XAVI_VAL_SYNTAX_ERR,    /* Error: A syntax error was
				    encountered. */
	XAVI_VAL_ZERO_DIV_ERR,  /* Error: Division by zero was
				    attempted. */
	XAVI_VAL_BAD_FUNCTION,  /* Error: A function call could not be
				    resolved to a valid function. */
	XAVI_VAL_BAD_ARGUMENTS, /* Error: A function call was made with
				    a number of arguments that the
				    function can't accept. */
	XAVI_VAL_DOMAIN_ERR,    /* Error: A function call resulted in a
				    domain error. */
	XAVI_VAL_RANGE_ERR      /* Error: A function call resulted in a
				    range error. */
};
typedef enum XaviValueStatus XaviValueStatus;

struct XAVI_API XaviValue
{
	XaviValueStatus Status;
	union
	{
		int Integer;
		float Float;
	};
};
typedef struct XaviValue XaviValue;

XAVI_API char *XaviValueToString(XaviValue InputValue);

#if defined __cplusplus
}
#endif

#endif /* XAVI_VALUE_H */
