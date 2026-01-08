// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Siliko.

// Siliko is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Siliko is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Siliko. If not, see
// <http://www.gnu.org/licenses/>.


#if !defined SILIKO_CORE_ENGINE_H
#define SILIKO_CORE_ENGINE_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Function.h>
#include <SilikoCore/Value.h>

typedef struct SilikoEngine SilikoEngine;

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT
SilikoEngine *SilikoEngineNew(void);
SILIKOCORE_EXPORT
void SilikoEngineDelete(SilikoEngine *object);

SILIKOCORE_EXPORT
SilikoValue *SilikoEngineCallFunction(
    SilikoEngine* object,
    const char *function_name,
    int argument_count,
    SilikoValue **arguments);
SILIKOCORE_EXPORT
SilikoFunction *SilikoEngineFetchFunction(
    SilikoEngine* object,
    const char *function_name);
SILIKOCORE_EXPORT
int SilikoEngineInstallFunction(
    SilikoEngine* object,
    const char *function_name,
    SilikoFunction *function);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_ENGINE_H
