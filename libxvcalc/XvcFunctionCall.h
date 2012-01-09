/*
 * XvcFunctionCall.h: Function handling.
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

#if !defined XVC_FUNCTIONS_H
#define XVC_FUNCTIONS_H

#include <setjmp.h>

#include "XvcStructs.h"

XvcNumber XvcFunctionCall(const char * FunctionName, int ArgumentCount, XvcNumber * Arguments, jmp_buf EscapePoint);
#endif // XVC_FUNCTIONS_H
