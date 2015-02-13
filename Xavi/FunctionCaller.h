/*
 * FunctionCaller.h: Function handling
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

#if !defined XAVI_FUNCTION_CALLER_H
#define XAVI_FUNCTION_CALLER_H

#include "W32Dll.h"
#include "Value.h"

#if defined __cplusplus
extern "C" {
#endif

typedef XaviValue (*XaviFunctionPointer)(int, XaviValue *);

XAVI_API int XaviFunctionCallerSetUp(void);
XAVI_API void XaviFunctionCallerTearDown(void);
XAVI_API XaviValue XaviFunctionCallerCall(const char *FunctionName, int ArgumentCount, XaviValue *Arguments);
XAVI_API int XaviFunctionCallerInstall(const char *FunctionName, XaviFunctionPointer Function);

#if defined __cplusplus
}
#endif

#endif // Xavi_FUNCTION_CALLER_H
