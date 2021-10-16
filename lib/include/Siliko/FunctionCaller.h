/* FunctionCaller.h: Function handling
 * Copyright 2012-2021 Vincent Damewood
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

#if !defined SILIKO_FUNCTION_CALLER_H
#define SILIKO_FUNCTION_CALLER_H

#include <Siliko/Api.h>
#include <Siliko/Value.h>

#if defined __cplusplus
extern "C" {
#endif

typedef SilikoValue (*SilikoFunctionPointer)(int, SilikoValue *);

SILIKO_API int SilikoFunctionCallerSetUp(void);
SILIKO_API void SilikoFunctionCallerTearDown(void);
SILIKO_API SilikoValue SilikoFunctionCallerCall(const char *FunctionName, int ArgumentCount, SilikoValue *Arguments);
SILIKO_API int SilikoFunctionCallerInstall(const char *FunctionName, SilikoFunctionPointer Function);

#if defined __cplusplus
}
#endif

#endif // SILIKO_FUNCTION_CALLER_H
